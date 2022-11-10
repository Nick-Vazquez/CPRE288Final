import logging
import tkinter as tk
import tkinter.ttk as ttk
import Components.NavBar as NavBar
import Components.MovementButtons as Buttons

app_screen_width_pct = 50
app_screen_height_pct = 50


def main():
    logging.basicConfig(level=logging.INFO)

    root = tk.Tk()
    style = ttk.Style(root)
    style.theme_use('clam')

    width = root.winfo_screenwidth() * (app_screen_width_pct / 100)
    height = root.winfo_screenheight() * (app_screen_height_pct / 100)
    root.geometry(f"{int(width)}x{int(height)}")

    window = tk.Frame(root)

    navbar = NavBar.NavBar(window)
    navbar.pack(fill=tk.X, expand=True)

    button = Buttons.MovementButtons(window)
    button.pack()

    window.pack(fill=tk.BOTH, expand=True)
    root.mainloop()


if __name__ == '__main__':
    main()
