"""Credit: https://beenje.github.io/blog/posts/logging-to-a-tkinter
-scrolledtext-widget/"""
import logging
import queue
import tkinter as tk
import tkinter.scrolledtext as scrolled

from Components.Console.QueueHandler import QueueHandler

logger = logging.getLogger(__name__)

UPDATE_TIME_MS = 100


class ConsoleUi(tk.Frame):
    """Poll messages from a logging queue and display
    them in a scrolled text widget"""

    def __init__(self, parent, *args, **kwargs):
        super(ConsoleUi, self).__init__(parent, *args, **kwargs)
        # Create a ScrolledText widget
        self.scrolled_text = scrolled.ScrolledText(self,
                                                   state='disabled', height=12)
        self.scrolled_text.grid(row=0, column=0, sticky="nsew")
        self.scrolled_text.configure(font='TkFixedFont')
        self.scrolled_text.tag_config('INFO', foreground='white')
        self.scrolled_text.tag_config('DEBUG', foreground='gray')
        self.scrolled_text.tag_config('WARNING', foreground='orange')
        self.scrolled_text.tag_config('ERROR', foreground='red')
        self.scrolled_text.tag_config('CRITICAL', foreground='red',
                                      underline=True)
        # Create a logging handler using a queue
        self.log_queue = queue.Queue()
        self.queue_handler = QueueHandler(self.log_queue)
        formatter = logging.Formatter('%(asctime)s - %(levelname)s - '
                                      '%(name)s: %(message)s')
        self.queue_handler.setFormatter(formatter)
        logger.addHandler(self.queue_handler)
        # Start polling messages from the queue
        self.after(UPDATE_TIME_MS, self.poll_log_queue)

    def display(self, record):
        msg = self.queue_handler.format(record)
        self.scrolled_text.configure(state='normal')
        self.scrolled_text.insert(tk.END, msg + '\n', record.levelname)
        self.scrolled_text.configure(state='disabled')
        # Autoscroll to the bottom
        self.scrolled_text.yview(tk.END)

    def poll_log_queue(self):
        # Check every 100ms if there is a new message in the queue to display
        while True:
            try:
                record = self.log_queue.get(block=False)
            except queue.Empty:
                break
            else:
                self.display(record)
        self.after(UPDATE_TIME_MS, self.poll_log_queue)
