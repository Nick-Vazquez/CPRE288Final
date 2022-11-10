import logging
import tkinter
import tkinter as tk
import Components.NavBar as NavBar
import Components.ScanPlotterView as Plotter
import Models.ScanResults as Results

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

    scan_result = Results.ScanResult()
    scan_result.result = [i/2 for i in range(90)]
    plotter = Plotter.PlotterView(window, scan_result)
    plotter.pack()

    window.pack(fill=tkinter.BOTH, expand=True)
    root.mainloop()


if __name__ == '__main__':
    main()
