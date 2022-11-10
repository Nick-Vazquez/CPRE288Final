import tkinter.ttk as ttk


def _generate_arrow_children(**kwargs):
    args = {'sticky': 'nswe', 'border': '1', **kwargs}
    return {'children': [('Button.padding', args)]}


class MovementButtons(ttk.Frame):
    """Buttons to control the physical bot movement"""
    def __init__(self, parent, *args, **kwargs):
        super(MovementButtons, self).__init__(parent, *args, **kwargs)

        self.set_style()
        self.draw()

    def set_style(self):
        style = ttk.Style()

        print(style.element_options('Button.leftarrow'))

        style.configure('Button.leftarrow', arrowsize=200)

        left_arr = {'children': [('Button.leftarrow', None)]}
        right_arr = {'children': [('Button.rightarrow', None)]}
        up_arr = {'children': [('Button.uparrow', None)]}
        down_arr = {'children': [('Button.downarrow', None)]}

        left_arrow = _generate_arrow_children(**left_arr)
        right_arrow = _generate_arrow_children(**right_arr)
        up_arrow = _generate_arrow_children(**up_arr)
        down_arrow = _generate_arrow_children(**down_arr)

        style.layout('Left.TButton', [('Button.focus', left_arrow)])
        style.layout('Right.TButton', [('Button.focus', right_arrow)])
        style.layout('Up.TButton', [('Button.focus', up_arrow)])
        style.layout('Down.TButton', [('Button.focus', down_arrow)])

    def draw(self):
        left_button = ttk.Button(self, style="Left.TButton", text='')
        right_button = ttk.Button(self, style="Right.TButton", text='')
        up_button = ttk.Button(self, style="Up.TButton", text='')
        down_button = ttk.Button(self, style="Down.TButton", text='')

        up_button.grid(row=0, column=0, columnspan=3)
        left_button.grid(row=1, column=0)
        down_button.grid(row=1, column=1)
        right_button.grid(row=1, column=2)
