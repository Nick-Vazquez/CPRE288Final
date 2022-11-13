import logging
import tkinter
import tkinter as tk
import Components.NavBar as NavBar
from Models.NavBarCallbacks import *

app_screen_width_pct = 50
app_screen_height_pct = 50


def main():
    logging.basicConfig(level=logging.INFO)

    root = tk.Tk()
    width = root.winfo_screenwidth() * (app_screen_width_pct / 100)
    height = root.winfo_screenheight() * (app_screen_height_pct / 100)
    root.geometry(f"{int(width)}x{int(height)}")

    window = tk.Frame(root)

    nav_callbacks = NavSectionCallbacks(lambda x=0: print('Home'),
                                        lambda x=0: print('Console'),
                                        lambda x=0: print('About'))
    control_callbacks = ControlSectionCallbacks(lambda x=0: print('Start'),
                                                lambda x=0: print('Stop'),
                                                lambda x=0: print('Pause'),
                                                lambda x=0: print('Reset'),
                                                lambda x=0: print('EStop'))
    navbar_callbacks = NavBarCallbacks(nav_callbacks, control_callbacks)
    navbar = NavBar.NavBar(window, navbar_callbacks)
    navbar.pack(fill=tkinter.X, expand=True)

    window.pack(fill=tkinter.BOTH, expand=True)
    root.mainloop()


if __name__ == '__main__':
    main()
