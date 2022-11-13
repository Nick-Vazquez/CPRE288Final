from typing import Callable


class MovementCallbacks:
    """Defines the callbacks used in a movement control widget."""
    def __init__(self, forward: Callable, reverse: Callable,
                 left: Callable, right: Callable):
        self._forward = forward
        self._reverse = reverse
        self._left = left
        self._right = right

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
    def left(self):
        return self._left

    @left.setter
    def left(self, value):
        self._left = value

    @property
    def right(self):
        return self._right

    @right.setter
    def right(self, value):
        self._right = value