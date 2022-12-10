""" Implementation of a logging handler to catch and route logs to the GUI
console.

Credit: https://beenje.github.io/blog/posts/logging-to-a-tkinter
-scrolledtext-widget/

__created__ = 2022/11/29
__author__ = Nick Vazquez (nmv)
"""
import logging
import tkinter as tk


class TextHandler(logging.Handler):
    """This class allows you to log to a Tkinter Text or ScrolledText widget"""

    def __init__(self, text):
        # run the regular Handler __init__
        logging.Handler.__init__(self)
        # Store a reference to the Text it will log to
        self.text = text

    def emit(self, record):
        """Overrides the default log handling with the updated formatter."""
        msg = self.format(record)

        def _append():
            self.text.configure(state='normal')
            self.text.insert(tk.END, msg + '\n')
            self.text.configure(state='disabled')
            # Autoscroll to the bottom
            self.text.yview(tk.END)
        # This is necessary because we can't modify the Text from other threads
        self.text.after(0, _append)
