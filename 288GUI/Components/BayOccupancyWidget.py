"""View for showing how many objects are detected in a bay.

__created__ = 2022/12/03
__author__ = Nick Vazquez (nmv)
"""
import tkinter as tk
import typing


class NumberWidget(tk.Frame):
    """Widget with a label and corresponding number. Displays occupancy of a
    bay."""
    def __init__(self, parent, label_text: str, tracked_number: tk.IntVar,
                 *args, **kwargs):
        kwargs.update(highlightthickness=2, highlightbackground="red",
                      bd=3, relief='raised', padx=15, pady=5)
        super(NumberWidget, self).__init__(parent, *args, **kwargs)
        self.text = label_text
        self.number = tracked_number

        self.draw()

    def draw(self):
        """Constructs the tkinter view."""
        label = tk.Label(self, text=self.text, font=('Helvetica', 16, 'bold'))
        label.pack()

        number = tk.Label(self, textvariable=self.number,
                          font=('Helvetica', 30, 'bold'))
        number.pack()


class BayOccupancyWidget(tk.Frame):
    """Application-specific grouping of NumberWidgets to display bay
    occupancies."""
    def __init__(self, parent, *args, **kwargs):
        super(BayOccupancyWidget, self).__init__(parent, *args, **kwargs)

        self._occupancies = [tk.IntVar(value=0) for _ in range(4)]

        self.draw()

    def draw(self):
        """Constructs the tkinter view."""
        number_1 = NumberWidget(self, "Bay 1", self.occupancies[0])
        number_2 = NumberWidget(self, "Bay 2", self.occupancies[1])
        number_3 = NumberWidget(self, "Bay 3", self.occupancies[2])
        number_4 = NumberWidget(self, "Bay 4", self.occupancies[3])

        number_1.grid(row=0, column=0)
        number_2.grid(row=0, column=1)
        number_3.grid(row=1, column=0)
        number_4.grid(row=1, column=1)

    @property
    def occupancies(self) -> typing.List[tk.IntVar]:
        """Collection of IntVars corresponding to each bay occupancy."""
        return self._occupancies

    @occupancies.setter
    def occupancies(self, values: typing.List[int],
                    bays: typing.List[int] = None):
        if bays is None:
            bays = [1, 2, 3, 4]
        if len(values) != len(bays):
            raise ValueError("Incorrect number of bays passed!")
        for index, bay in enumerate(bays):
            self._occupancies[bay-1].set(values[index])
