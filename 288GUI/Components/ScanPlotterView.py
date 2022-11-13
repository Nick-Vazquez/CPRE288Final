"""
__created__ = 2022/11/10
__author__ = Nick Vazquez (nmv)
"""
import tkinter as tk
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

from Models import ScanResults


class PlotterView(tk.Frame):
    """Plotting view widget to show scan results."""
    def __init__(self, parent, scan_results: ScanResults.ScanResult,
                 *args, **kwargs):
        super(PlotterView, self).__init__(parent, *args, **kwargs)

        self.results = scan_results
        self.fig = plt.figure()
        self.ax = None

        canvas = FigureCanvasTkAgg(self.fig, master=self)
        canvas.get_tk_widget().grid(row=0, column=0)

        animate = FuncAnimation(self.fig, self.animate, interval=500)
        plt.show()

    def animate(self, i):
        """Tkinter input for auto-updating from result data."""
        result_df = self.results.to_dataframe()

        if self.ax:
            plt.delaxes(self.ax)
        self.ax = self.fig.add_subplot(111, polar=True)

        self.ax.plot(result_df['rad'], result_df['value'])
        self.ax.set_rmax(30)
        self.ax.set_rticks([0, 5, 10, 15, 20, 25, 30])

        self.ax.set_thetamin(0)
        self.ax.set_thetamax(180)
        self.ax.set_title("IR Sensor Data", va='bottom')
