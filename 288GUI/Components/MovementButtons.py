import tkinter.ttk as ttk


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
    def __init__(self, parent, *args, **kwargs):
        super(MovementButtons, self).__init__(parent, *args, **kwargs)

        self.set_style()
        self.draw()

    def set_style(self):
        style = ttk.Style()

        style.layout(*_generate_arrow_children('Left', 'leftarrow'))
        style.layout(*_generate_arrow_children('Right', 'rightarrow'))
        style.layout(*_generate_arrow_children('Up', 'uparrow'))
        style.layout(*_generate_arrow_children('Down', 'downarrow'))

    def draw(self):
        left_button = ttk.Button(self, style="Left.TButton", text='')
        right_button = ttk.Button(self, style="Right.TButton", text='')
        up_button = ttk.Button(self, style="Up.TButton", text='')
        down_button = ttk.Button(self, style="Down.TButton", text='')

        up_button.grid(row=0, column=0, columnspan=3)
        left_button.grid(row=1, column=0)
        down_button.grid(row=1, column=1)
        right_button.grid(row=1, column=2)
