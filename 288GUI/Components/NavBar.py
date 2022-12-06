"""
__created__ = 2022/11/09
__author__ = Nick Vazquez (nmv)

The reason for each separate callback method is the updated callback method
from the class dict won't be updated if just looking up the dict. Without the
special functions the warning is run even if callback is set.
"""
import abc
import logging
import tkinter as tk
import typing

from Models.NavBarCallbacks import *


class ButtonComponent(abc.ABC, tk.Frame):
    """Component with buttons that have callbacks."""
    @abc.abstractmethod
    def __init__(self, parent, *args, **kwargs):
        super(ButtonComponent, self).__init__(parent, *args, **kwargs)
        self.logger = logging.getLogger(__name__)
        self._callbacks: typing.Dict[str, typing.Callable] = {}

    def _warn_unimplemented_callback(self):
        self.logger.warning(f"Unimplemented callback!. \n"
                            f"Please make sure to set callbacks "
                            f"for nav buttons.")

    @abc.abstractmethod
    def draw(self):
        """Renders the initial frame components."""
        pass

    @property
    def callbacks(self):
        """All callbacks for the component."""
        return self._callbacks

    @callbacks.setter
    def callbacks(self, value: typing.Dict[str, typing.Callable]):
        self._callbacks = value


class NavBar(tk.Frame):
    """Navigation and application control bar at the top of the app."""
    def __init__(self, parent, nav_callbacks: NavBarCallbacks, *args, **kwargs):
        self.logger = logging.getLogger(__name__)

        self._nav_callbacks: NavBarCallbacks = nav_callbacks

        styling = {
            "relief": "raised",
            "borderwidth": 3,
            "background": "red"
        }
        kwargs.update(**styling)
        super().__init__(parent, *args, **kwargs)

        self.draw_navbar()

    def start_program(self):
        """Temp start callback"""
        self.logger.info("STARTING!")

    def stop_program(self):
        """Temp stop callback"""
        self.logger.info("STOPPING!")

    def draw_navbar(self):
        """Populates the self frame with NavBar contents."""
        self.columnconfigure(1, weight=1)

        nav_section = NavSection(self, self._nav_callbacks.nav_callbacks)
        nav_section.grid(row=0, column=0)

        opmode_section = OpModeSelector(self,
                                        self._nav_callbacks.opmode_callbacks)
        opmode_section.grid(row=0, column=1)

        control_section = ControlSection(self,
                                         self._nav_callbacks.control_callbacks)
        control_section.grid(row=0, column=2, sticky=tk.E)


class NavSection(ButtonComponent):
    """Section to switch between pages in the app."""
    def __init__(self, parent, nav_callbacks: NavSectionCallbacks,
                 *args, **kwargs):
        super(NavSection, self).__init__(parent, *args, **kwargs)

        self._callbacks: NavSectionCallbacks = nav_callbacks

        self.draw()

    def draw(self):
        """Populates initial widgets for navigation."""
        app_name = tk.Label(self, text="Warehouse Mapper Buddy")
        home_button = tk.Button(self, text="Home",
                                command=self.home_callback)
        console_button = tk.Button(self, text="Console",
                                   command=self.console_callback)
        about_button = tk.Button(self, text="About",
                                 command=self.about_callback)

        app_name.grid(row=0, column=0)
        home_button.grid(row=0, column=1)
        console_button.grid(row=0, column=2)
        about_button.grid(row=0, column=3)

    def home_callback(self):
        """Callback invoked when home button is pressed"""
        self._callbacks.home()

    def console_callback(self):
        """Callback invoked when home button is pressed"""
        self._callbacks.console()

    def about_callback(self):
        """Callback invoked when home button is pressed"""
        self._callbacks.about()


class OpModeSelector(ButtonComponent):
    def __init__(self, parent, callbacks: OpModeCallbacks,
                 *args, **kwargs):
        super(OpModeSelector, self).__init__(parent, *args, **kwargs)
        self._callbacks: OpModeCallbacks = callbacks
        self.draw()

    def draw(self):
        teleop = tk.Button(self, text="TeleOp", command=self._callbacks.teleop)
        auto = tk.Button(self, text="Autonomous", command=self._callbacks.auto)

        teleop.grid(row=0, column=0)
        auto.grid(row=0, column=1)


class ControlSection(ButtonComponent):
    """Section for controlling the bot/mission."""
    def __init__(self, parent, control_callbacks: ControlSectionCallbacks,
                 *args, **kwargs):
        super(ControlSection, self).__init__(parent, *args, **kwargs)

        self._callbacks: ControlSectionCallbacks = control_callbacks

        self.draw()

    def draw(self):
        """Draws the application/bot control buttons"""
        start_button = tk.Button(self, text="Start")
        start_button.bind("<Button-1>", self.start_button_callback)

        start_button.grid(row=0, column=0)

    def remove_stop_button(self, event):
        """Removes the stop button placed on top of the start button"""
        stop_button = event.widget
        start_button = tk.Button(self, text="Start")
        start_button.bind("<Button-1>", self.start_button_callback)
        stop_button.destroy()

    def place_stop_button(self, event):
        """Stacks a stop button over the start button that triggered event."""
        widget = event.widget
        stop_button = tk.Button(widget, text="Stop")
        stop_button.bind("<Button-1>", self.stop_button_callback)
        stop_button.place(x=0, y=0, anchor="nw", relwidth=1.0, relheight=1.0)
        stop_button.focus_set()

    def stop_button_callback(self, event):
        """Callback to invoke when the stop button is pressed"""
        self.remove_stop_button(event)
        self._callbacks.stop()

    def start_button_callback(self, event):
        """Callback to invoke when the start button is pressed"""
        self.place_stop_button(event)
        self._callbacks.start()
