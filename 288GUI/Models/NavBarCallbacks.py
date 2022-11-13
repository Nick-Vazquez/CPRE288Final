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
    _pause: Callable[..., Any]
    _reset: Callable[..., Any]
    _estop: Callable[..., Any]

    def __init__(self, start: Callable, stop: Callable, pause: Callable,
                 reset: Callable, estop: Callable):
        self._start = start
        self._stop = stop
        self._pause = pause
        self._reset = reset
        self._estop = estop

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

    @property
    def pause(self):
        return self._pause

    @pause.setter
    def pause(self, value: Callable):
        self._pause = value

    @property
    def reset(self):
        return self._reset

    @reset.setter
    def reset(self, value: Callable):
        self._reset = value

    @property
    def estop(self):
        return self._estop

    @estop.setter
    def estop(self, value: Callable):
        self._estop = value


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
