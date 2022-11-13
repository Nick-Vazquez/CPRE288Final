import logging
import tkinter as tk
import tkinter.ttk as ttk
from Components.NumberWidget import NumberWidget

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

    number = NumberWidget(window, "Left", tk.IntVar(value=5))
    number.pack()

    window.pack(fill=tk.BOTH, expand=True)
    root.mainloop()


if __name__ == '__main__':
    main()
