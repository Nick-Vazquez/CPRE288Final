import tkinter as tk
import tkinter.ttk as ttk

from Components.Console.Clock import Clock
from Components.Console.ConsoleUI import ConsoleUi


class Console:
    def create_console(self, root: tk.Frame) -> ttk.Frame:
        console_frame = ttk.Frame(root)

        self.console = ConsoleUi(console_frame)
        self.clock = Clock()
        self.clock.start()
        root.protocol('WM_DELETE_WINDOW', self.quit)
        return console_frame
