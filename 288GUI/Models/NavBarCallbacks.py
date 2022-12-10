"""Defines models that are used to hold application control callbacks.

__created__ = 2022/11/12
__author__ = Nick Vazquez (nmv)
"""
from typing import Callable, Any


class NavSectionCallbacks:
    """Callbacks used to navigate from application page to page."""
    def __init__(self, home: Callable, console: Callable, about: Callable):
        self._home = home
        self._console = console
        self._about = about

    @property
    def home(self):
        """Callback to return to the home (main) screen."""
        return self._home

    @home.setter
    def home(self, value: Callable):
        self._home = value

    @property
    def console(self):
        """Callback to get to a console-only screen."""
        return self._console

    @console.setter
    def console(self, value: Callable):
        self._console = value

    @property
    def about(self):
        """Callback to get to the about screen."""
        return self._about

    @about.setter
    def about(self, value: Callable):
        self._about = value


class ControlSectionCallbacks:
    """Defines callbacks used to control mission execution on the bot."""
    _start: Callable[..., Any]
    _stop: Callable[..., Any]

    def __init__(self, start: Callable, stop: Callable):
        self._start = start
        self._stop = stop

    @property
    def start(self):
        """Start function to command bot start"""
        return self._start

    @start.setter
    def start(self, value: Callable):
        self._start = value

    @property
    def stop(self):
        """Start function to command bot stop"""
        return self._stop

    @stop.setter
    def stop(self, value: Callable):
        self._stop = value


class OpModeCallbacks:
    """Defines callbacks to be called when the driver selects an operation
    mode."""
    _teleop: Callable[..., Any]
    _auto: Callable[..., Any]

    def __init__(self, teleop: Callable, auto: Callable):
        self._teleop = teleop
        self._auto = auto

    @property
    def teleop(self):
        """Commands a bot to enter driver-controlled mode."""
        return self._teleop

    @teleop.setter
    def teleop(self, value: Callable):
        self._teleop = value

    @property
    def auto(self):
        """Commands a bot to enter autonomous mode."""
        return self._auto

    @auto.setter
    def auto(self, value: Callable):
        self._auto = value


class NavBarCallbacks:
    """Contains all callback classes used within the top navbar."""
    _nav_callbacks: NavSectionCallbacks
    _opmode_callbacks: OpModeCallbacks
    _control_callbacks: ControlSectionCallbacks

    def __init__(self, nav_callbacks: NavSectionCallbacks,
                 opmode_callbacks: OpModeCallbacks,
                 control_callbacks: ControlSectionCallbacks):
        self._nav_callbacks = nav_callbacks
        self._opmode_callbacks = opmode_callbacks
        self._control_callbacks = control_callbacks

    @property
    def nav_callbacks(self):
        """Callbacks of the application page navigation section."""
        return self._nav_callbacks

    @nav_callbacks.setter
    def nav_callbacks(self, value: NavSectionCallbacks):
        self._nav_callbacks = value

    @property
    def opmode_callbacks(self):
        """Callbacks of the operational mode selector section."""
        return self._opmode_callbacks

    @opmode_callbacks.setter
    def opmode_callbacks(self, value):
        self._opmode_callbacks = value

    @property
    def control_callbacks(self):
        """Callbacks of the start/stop section."""
        return self._control_callbacks

    @control_callbacks.setter
    def control_callbacks(self, value: ControlSectionCallbacks):
        self._control_callbacks = value
