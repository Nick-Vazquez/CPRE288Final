import logging
import sys
import tkinter as tk
import tkinter.ttk as ttk
import Components.NavBar as NavBar
import Components.ScanPlotterView as Plotter
import Components.MovementButtons as Buttons
from Components.NumberWidget import NumberWidget
from Models.MovementCallbacks import MovementCallbacks
import Models.ScanResults as Results
from Models.NavBarCallbacks import *
from Services import CommunicationService
from Services.MovementService import MovementService
from Services.SerialService import SerialService

app_screen_width_pct = 75
app_screen_height_pct = 75


def main():
    logging.basicConfig(level=logging.INFO)

    root = tk.Tk()
    style = ttk.Style(root)
    style.theme_use('clam')

    width = root.winfo_screenwidth() * (app_screen_width_pct / 100)
    height = root.winfo_screenheight() * (app_screen_height_pct / 100)
    root.geometry(f"{int(width)}x{int(height)}")
    root.title("Parking Buddy")

    window = tk.Frame(root)

    serial_service: CommunicationService = SerialService()

    try:
        serial_service.establish_connection()
    except ConnectionRefusedError:
        logging.warning("Could not establish a connection to the CyBot!")
        sys.exit(1)

    movement_service = MovementService(serial_service)

    nav_callbacks = NavSectionCallbacks(lambda x=0: print('Home'),
                                        lambda x=0: print('Console'),
                                        lambda x=0: print('About'))

    control_callbacks = ControlSectionCallbacks(lambda x=0: print('Start'),
                                                lambda x=0: print('Stop'),
                                                lambda x=0: print('Pause'),
                                                lambda x=0: print('Reset'),
                                                lambda x=0: print('EStop'))

    movement_callbacks = MovementCallbacks(movement_service.forward,
                                           movement_service.reverse,
                                           movement_service.c_clockwise,
                                           movement_service.clockwise)

    navbar_callbacks = NavBarCallbacks(nav_callbacks, control_callbacks)
    navbar = NavBar.NavBar(window, navbar_callbacks)
    navbar.pack(fill=tk.X, expand=True)

    button = Buttons.MovementButtons(window, movement_callbacks)
    button.pack()

    scan_result = Results.ScanResult()
    scan_result.result = [i / 2 for i in range(90)]
    plotter = Plotter.PlotterView(window, scan_result)
    plotter.pack()

    number_frame = tk.Frame(window)
    number_1 = NumberWidget(number_frame, "Left", tk.IntVar(value=5))
    number_2 = NumberWidget(number_frame, "Center", tk.IntVar(value=2))
    number_3 = NumberWidget(number_frame, "Right", tk.IntVar(value=10))

    number_1.grid(row=0, column=0)
    number_2.grid(row=0, column=1)
    number_3.grid(row=0, column=2)

    number_frame.pack()

    window.pack(fill=tk.BOTH, expand=True)
    root.mainloop()


if __name__ == '__main__':
    main()
