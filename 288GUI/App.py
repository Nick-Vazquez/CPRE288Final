import logging
import tkinter as tk
import tkinter.ttk as ttk
import Components.NavBar as NavBar
import Components.ScanPlotterView as Plotter
import Components.MovementButtons as Buttons
from Components.BayOccupancyWidget import BayOccupancyWidget
from Models.MovementCallbacks import MovementCallbacks
import Models.ScanResults as Results
from Models.NavBarCallbacks import *

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

    nav_callbacks = NavSectionCallbacks(lambda x=0: print('Home'),
                                        lambda x=0: print('Console'),
                                        lambda x=0: print('About'))
    control_callbacks = ControlSectionCallbacks(lambda x=0: print('Start'),
                                                lambda x=0: print('Stop'),
                                                lambda x=0: print('Pause'),
                                                lambda x=0: print('Reset'),
                                                lambda x=0: print('EStop'))
    movement_callbacks = MovementCallbacks(lambda x=0: print("Forward"),
                                           lambda x=0: print("Reverse"),
                                           lambda x=0: print("Left"),
                                           lambda x=0: print("Right"))

    navbar_callbacks = NavBarCallbacks(nav_callbacks, control_callbacks)
    navbar = NavBar.NavBar(window, navbar_callbacks)
    navbar.pack(fill=tk.X, expand=True)

    button = Buttons.MovementButtons(window, movement_callbacks)
    button.pack()

    scan_result = Results.ScanResult()
    scan_result.result = [i / 2 for i in range(90)]
    plotter = Plotter.PlotterView(window, scan_result)
    plotter.pack()

    occupancy_widget = BayOccupancyWidget(window)
    occupancy_widget.occupancies = [3, 4, 5, 6]
    occupancy_widget.pack()

    window.pack(fill=tk.BOTH, expand=True)
    root.mainloop()


if __name__ == '__main__':
    main()
