import tkinter.ttk as ttk

from Models.MovementCallbacks import MovementCallbacks


def _generate_arrow_children(style_name: str, arrow_dir: str):
    return f'{style_name}.TButton', [
        ('Button.focus', {'children': [
            (f'Button.{arrow_dir}', None),
            ('Button.padding', {'sticky': 'nswe', 'children': [
                ('Button.label', {'sticky': 'nswe'}
                 )]}
             )]}
         )]


class MovementButtons(ttk.Frame):
    """Buttons to control the physical bot movement"""
    def __init__(self, parent, callbacks: MovementCallbacks, *args, **kwargs):
        super(MovementButtons, self).__init__(parent, *args, **kwargs)

        self.callbacks = callbacks

        self.set_callbacks()
        self.set_style()
        self.draw()

    def set_callbacks(self):
        self.bind_all('w', lambda x: self.callbacks.forward())
        self.bind_all('s', lambda x: self.callbacks.reverse())
        self.bind_all('a', lambda x: self.callbacks.c_clockwise())
        self.bind_all('d', lambda x: self.callbacks.clockwise())

    def set_style(self):
        style = ttk.Style()

        style.layout(*_generate_arrow_children('Left', 'leftarrow'))
        style.layout(*_generate_arrow_children('Right', 'rightarrow'))
        style.layout(*_generate_arrow_children('Up', 'uparrow'))
        style.layout(*_generate_arrow_children('Down', 'downarrow'))

    def draw(self):
        left_button = ttk.Button(self, style="Left.TButton", text='',
                                 command=self.callbacks.c_clockwise)
        right_button = ttk.Button(self, style="Right.TButton", text='',
                                  command=self.callbacks.clockwise)
        up_button = ttk.Button(self, style="Up.TButton", text='',
                               command=self.callbacks.forward)
        down_button = ttk.Button(self, style="Down.TButton", text='',
                                 command=self.callbacks.reverse)

        up_button.grid(row=0, column=0, columnspan=3)
        left_button.grid(row=1, column=0)
        down_button.grid(row=1, column=1)
        right_button.grid(row=1, column=2)
