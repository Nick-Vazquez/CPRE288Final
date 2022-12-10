"""
Callbacks to be used with views to define button and other widget callbacks.
"""
from typing import Callable


# noinspection PyMissingOrEmptyDocstring
class MovementCallbacks:
    """Defines the callbacks used in a movement control widget."""
    def __init__(self, forward: Callable, reverse: Callable,
                 c_clockwise: Callable, clockwise: Callable):
        self._forward = forward
        self._reverse = reverse
        self._left = c_clockwise
        self._right = clockwise

    @property
    def forward(self):
        return self._forward

    @forward.setter
    def forward(self, value):
        self._forward = value

    @property
    def reverse(self):
        return self._reverse

    @reverse.setter
    def reverse(self, value):
        self._reverse = value

    @property
    def c_clockwise(self):
        return self._left

    @c_clockwise.setter
    def c_clockwise(self, value):
        self._left = value

    @property
    def clockwise(self):
        return self._right

    @clockwise.setter
    def clockwise(self, value):
        self._right = value
