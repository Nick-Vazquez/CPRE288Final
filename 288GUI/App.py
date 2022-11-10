import logging
import tkinter
import tkinter as tk
import Components.NavBar as NavBar

app_screen_width_pct = 50
app_screen_height_pct = 50


def main():
    logging.basicConfig(level=logging.INFO)

    root = tk.Tk()
    width = root.winfo_screenwidth() * (app_screen_width_pct / 100)
    height = root.winfo_screenheight() * (app_screen_height_pct / 100)
    root.geometry(f"{int(width)}x{int(height)}")

    window = tk.Frame(root)

    navbar = NavBar.NavBar(window)
    navbar.pack(fill=tkinter.X, expand=True)

    window.pack(fill=tkinter.BOTH, expand=True)
    root.mainloop()


if __name__ == '__main__':
    main()