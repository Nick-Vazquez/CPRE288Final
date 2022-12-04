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

    def test_set_results_from_angles_distances(self):
        angles, distances = self.setup_result_angles()
        self.sut.set_result_from_angles_distances(angles, distances)
        self.assertEqual(self.sut.result, [5, 30, 25, 35])

    def test_set_results_incorrect_dists_throws_arithmetic_exception(self):
        angles, distances = self.setup_result_angles()
        with self.assertRaises(ArithmeticError):
            self.sut.set_result_from_angles_distances(angles, distances, 2)

    def test_set_results_from_angles_distances_step(self):
        self.sut.result = [2 for _ in range(20)]
        angles = [9, 18, 27]
        distances = [1, 1, 1]
        self.sut.set_result_from_angles_distances(angles, distances, 9)
        expected = [2, 1, 1, 1] + [2 for _ in range(16)]
        self.assertEqual(self.sut.result, expected)
