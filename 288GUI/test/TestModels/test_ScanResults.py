from unittest import TestCase
from Models.ScanResults import ScanResult


class TestScanResult(TestCase):
    def setUp(self) -> None:
        self.sut = ScanResult()

    def test_empty_results(self):
        self.assertEqual(self.sut.result, [])

    def test_set_results_at_starting_angle(self):
        self.sut.result = [5, 10, 15, 20]
        self.sut.set_result_at_starting_angle(1, [2, 4])
        self.assertEqual(self.sut.result, [5, 2, 4, 20])

    def setup_result_angles(self):
        self.sut.result = [5, 10, 25, 20]
        angles = [1, 3]
        distances = [30, 35]
        return angles, distances
