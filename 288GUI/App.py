"""Main GUI Application to interface with CyBot.

__created__ = 2022/11/09
__author__ = Nick Vazquez (nmv)
"""
import logging
import queue
import random
import sys
import signal
import tkinter as tk
import tkinter.ttk as ttk

import Components.MovementButtons as Buttons
from Components.Console.ConsoleUI import ConsoleUi
from Controllers.BayController import BayController
from Controllers.NavBarController import NavBarController

from Controllers.PlotterController import PlotterController

from Models.MovementCallbacks import MovementCallbacks

from Services import CommunicationService
from Services.CyBotMessageService import CyBotMessageService
from Services.MovementService import MovementService
from Services.OpModeService import OpModeService
from Services.SerialService import SerialService

app_screen_width_pct = 100
app_screen_height_pct = 100

logger = logging.getLogger()

MESSAGE_UPDATE_TIME_MS = 100


class App:
    """Main Tkinter application."""
    def __init__(self, root: tk.Tk):
        # Setup window and nav bar
        self.root = root
        style = ttk.Style(self.root)
        style.theme_use('clam')

        width = self.root.winfo_screenwidth() * (app_screen_width_pct / 100)
        height = self.root.winfo_screenheight() * (app_screen_height_pct / 100)
        self.root.geometry(f"{int(width)}x{int(height)}")
        self.root.title("Parking Buddy")

        window = tk.Frame(self.root)

        console = ConsoleUi(window)
        logger.addHandler(console.queue_handler)

        self.serial_service: CommunicationService = SerialService()
        self.incoming_message_queue: queue.Queue = queue.Queue()

        try:
            self.serial_service.establish_connection()
            self.root.after(0, self.serial_service.setup_poll,
                            self.incoming_message_queue)
        except ConnectionRefusedError:
            logging.warning("Could not establish a connection to the CyBot!")
            self.root.destroy()
            sys.exit(1)

        self.message_service: CyBotMessageService = \
            CyBotMessageService(self.incoming_message_queue)
        self.root.after(0, self.message_service.start)

        self.movement_service = MovementService(self.serial_service)
        self.opmode_service = OpModeService(self.serial_service)

        movement_callbacks = MovementCallbacks(
            self.movement_service.forward, self.movement_service.reverse,
            self.movement_service.c_clockwise,
            self.movement_service.clockwise)

        navbar = NavBarController(window, self.opmode_service)
        button = Buttons.MovementButtons(window, movement_callbacks)

        # Create plotter and subscribe it to incoming messages.
        scan_plotter = PlotterController(window)
        scan_plotter.ir_results.result = \
            [random.randint(1, 5) for _ in range(90)]
        self.message_service.subscribe(scan_plotter)

        occupancy_controller = BayController(window)
        self.message_service.subscribe(occupancy_controller)

        navbar.view.grid(row=0, column=0, columnspan=2, sticky="nsew")
        scan_plotter.view.grid(row=1, column=0, sticky="nsew")
        button.grid(row=1, column=1, sticky="nsew")
        occupancy_controller.view.grid(row=2, column=0, sticky="nsew")
        console.grid(row=3, column=0, columnspan=2, sticky="nsew")

        window.pack(fill=tk.BOTH, expand=True)

        self.root.protocol('WM_DELETE_WINDOW', self.quit)
        self.root.bind('<Control-q>', self.quit)
        signal.signal(signal.SIGINT, self.quit)

    def quit(self, *args):
        """Gracefully exits the program, destroying created threads."""
        self.root.destroy()


def main():
    """Starts the GUI app and main thread. Only runs if directly called."""
    logging.basicConfig(level=logging.DEBUG)
    logging.getLogger('matplotlib.font_manager').setLevel(logging.ERROR)
    root = tk.Tk()
    app = App(root)
    app.root.mainloop()


if __name__ == '__main__':
    main()
