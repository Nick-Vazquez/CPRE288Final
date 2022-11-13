import tkinter as tk


class NumberWidget(tk.Frame):
    def __init__(self, parent, label_text: str, tracked_number: tk.IntVar,
                 *args, **kwargs):
        kwargs.update(highlightthickness=2, highlightbackground="red",
                      bd=3, relief='raised', padx=15, pady=5)
        super(NumberWidget, self).__init__(parent, *args, **kwargs)
        self.text = label_text
        self.number = tracked_number

        self.draw()

    def draw(self):
        label = tk.Label(self, text=self.text, font=('Helvetica', 16, 'bold'))
        label.pack()

        number = tk.Label(self, textvariable=self.number,
                          font=('Helvetica', 30, 'bold'))
        number.pack()
