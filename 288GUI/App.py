import logging
import queue
import sys
import signal
import threading
import tkinter as tk
import tkinter.ttk as ttk
import typing

import Components.NavBar as NavBar
import Components.ScanPlotterView as Plotter
import Components.MovementButtons as Buttons
from Components.BayOccupancyWidget import BayOccupancyWidget
from Components.Console.ConsoleUI import ConsoleUi
from Models.MovementCallbacks import MovementCallbacks
import Models.ScanResults as Results
from Models.NavBarCallbacks import *
from Services import CommunicationService
from Services.CyBotMessageService import CyBotMessageService
from Services.MovementService import MovementService
from Services.SerialService import SerialService

app_screen_width_pct = 75
app_screen_height_pct = 75

logger = logging.getLogger()


class App:
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

        navbar_callbacks, movement_callbacks = self.setup_callbacks()
        navbar = NavBar.NavBar(window, navbar_callbacks)
        navbar.pack(fill=tk.X, expand=True)

        button = Buttons.MovementButtons(window, movement_callbacks)
        button.pack()

        console.pack(expand=True, fill=tk.X)

        scan_result = Results.ScanResult()
        scan_result.result = [i / 2 for i in range(90)]
        plotter = Plotter.PlotterView(window, scan_result)
        plotter.pack()

        occupancy_widget = BayOccupancyWidget(window)
        occupancy_widget.occupancies = [3, 4, 5, 6]
        occupancy_widget.pack()

        window.pack(fill=tk.BOTH, expand=True)

        self.root.protocol('WM_DELETE_WINDOW', self.quit)
        self.root.bind('<Control-q>', self.quit)
        signal.signal(signal.SIGINT, self.quit)

    def setup_callbacks(self):
        nav_callbacks = NavSectionCallbacks(lambda x=0: print('Home'),
                                            lambda x=0: print('Console'),
                                            lambda x=0: print('About'))
        control_callbacks = ControlSectionCallbacks(lambda x=0: print('Start'),
                                                    lambda x=0: print('Stop'))
        movement_callbacks = MovementCallbacks(
            self.movement_service.forward, self.movement_service.reverse,
            self.movement_service.c_clockwise, self.movement_service.clockwise)

        navbar_callbacks = NavBarCallbacks(nav_callbacks, control_callbacks)

        return navbar_callbacks, movement_callbacks

    def quit(self, *args):
        self.root.destroy()


def main():
    logging.basicConfig(level=logging.INFO)
    logging.getLogger('matplotlib.font_manager').setLevel(logging.ERROR)
    root = tk.Tk()
    app = App(root)
    app.root.mainloop()


if __name__ == '__main__':
    main()
