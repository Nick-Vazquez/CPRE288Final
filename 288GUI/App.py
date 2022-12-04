import logging
import sys
import signal
import tkinter as tk
import tkinter.ttk as ttk
import Components.NavBar as NavBar
import Components.ScanPlotterView as Plotter
import Components.MovementButtons as Buttons
from Components.BayOccupancyWidget import BayOccupancyWidget
from Components.Console import ConsoleUi
from Models.MovementCallbacks import MovementCallbacks
import Models.ScanResults as Results
from Models.NavBarCallbacks import *
from Services import CommunicationService
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

        serial_service: CommunicationService = SerialService()

        try:
            serial_service.establish_connection()
        except ConnectionRefusedError:
            logging.warning("Could not establish a connection to the CyBot!")
            sys.exit(1)

        movement_service = MovementService(serial_service)

        navbar_callbacks, movement_callbacks = self.setup_callbacks()
        navbar = NavBar.NavBar(window, navbar_callbacks)
        navbar.pack(fill=tk.X, expand=True)

        button = Buttons.MovementButtons(window, movement_callbacks)
        button.pack()

        console = ConsoleUi(window)
        console.pack()
        logger.addHandler(console.queue_handler)

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

    @staticmethod
    def setup_callbacks():
        nav_callbacks = NavSectionCallbacks(lambda x=0: print('Home'),
                                            lambda x=0: print('Console'),
                                            lambda x=0: print('About'))
        control_callbacks = ControlSectionCallbacks(lambda x=0: print('Start'),
                                                    lambda x=0: print('Stop'))
        movement_callbacks = MovementCallbacks(lambda x=0: print("Forward"),
                                               lambda x=0: print("Reverse"),
                                               lambda x=0: print("Left"),
                                               lambda x=0: print("Right"))

        navbar_callbacks = NavBarCallbacks(nav_callbacks, control_callbacks)

        return navbar_callbacks, movement_callbacks

    def quit(self, *args):
        self.root.destroy()


def main():
    logging.basicConfig(level=logging.INFO)
    root = tk.Tk()
    app = App(root)
    app.root.mainloop()


if __name__ == '__main__':
    main()
