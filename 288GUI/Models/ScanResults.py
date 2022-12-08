"""
__created__ = 2022/11/10
__author__ = Nick Vazquez (nmv)
"""
import logging
import math

import pandas as pd
import numpy as np

from typing import List


class ScanResult:
    """Model for storing results from a CyBOT Scan."""

    def __init__(self):
        self.logger = logging.getLogger(__name__)
        self._result: List = []

    @property
    def result(self):
        """Values of scan reading."""
        return self._result

    @result.setter
    def result(self, value: List[float]):
        # TODO: Add error checking or individual item assignment
        self._result = value

    def set_result_at_angle(self, angle: int, value: float):
        """Sets the result value at a particular angle (index)."""
        self._result[angle] = value

    def set_result_at_starting_angle(self, start_angle: int,
                                     values: List[float]):
        num_vals = len(values)
        self._result[start_angle:start_angle + num_vals] = values

    def to_dataframe(self) -> pd.DataFrame:
        """Returns the model in a DataFrame format."""
        result_df = pd.DataFrame(columns=["angle", "value"])
        # Set a radian column with the angle from each row
        result_df.angle = np.arange(0, 180, 2)
        result_df['rad'] = result_df.apply(
            lambda row: row.angle * math.pi / 180, axis=1)
        # Populate the DF with values
        result_df.value = self.result
        return result_df
