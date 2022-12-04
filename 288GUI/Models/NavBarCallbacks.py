from typing import Callable, Any


class NavSectionCallbacks:
    def __init__(self, home: Callable, console: Callable, about: Callable):
        self._home = home
        self._console = console
        self._about = about

    @property
    def home(self):
        return self._home

    @home.setter
    def home(self, value: Callable):
        self._home = value

    @property
    def console(self):
        return self._console

    @console.setter
    def console(self, value: Callable):
        self._console = value

    @property
    def about(self):
        return self._about

    @about.setter
    def about(self, value: Callable):
        self._about = value


class ControlSectionCallbacks:
    _start: Callable[..., Any]
    _stop: Callable[..., Any]

    def __init__(self, start: Callable, stop: Callable):
        self._start = start
        self._stop = stop

    @property
    def start(self):
        return self._start

    @start.setter
    def start(self, value: Callable):
        self._start = value

    @property
    def stop(self):
        return self._stop

    @stop.setter
    def stop(self, value: Callable):
        self._stop = value


class NavBarCallbacks:
    _nav_callbacks: NavSectionCallbacks
    _control_callbacks: ControlSectionCallbacks

    def __init__(self, nav_callbacks: NavSectionCallbacks,
                 control_callbacks: ControlSectionCallbacks):
        self._nav_callbacks = nav_callbacks
        self._control_callbacks = control_callbacks

    @property
    def nav_callbacks(self):
        return self._nav_callbacks

    @nav_callbacks.setter
    def nav_callbacks(self, value: NavSectionCallbacks):
        self._nav_callbacks = value

    @property
    def control_callbacks(self):
        return self._control_callbacks

    @control_callbacks.setter
    def control_callbacks(self, value: ControlSectionCallbacks):
        self._control_callbacks = value
