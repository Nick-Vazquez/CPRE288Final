"""
Tests for translating and sending CyBot messages.

__created__ = 2022/11/18
__author__ = Nick Vazquez (nmv)
"""
from unittest import TestCase

import Models.CyBotMessage
import Models.CyBotMessage as Message


class TestScanMessage(TestCase):
    def setUp(self) -> None:
        """Creates dummy data to be used in tests, and the associated model."""
        self.in_data = dict(update_type=Message.ScanUpdateType.IR,
                            angles=[120], distances=[59.5])
        self.model = Models.CyBotMessage.ScanResultsMessage(**self.in_data)

    def test_msg_type(self):
        # Assert
        self.assertEqual(self.model.mes_type, 4)

    def test_update_type(self):
        self.assertEqual(self.model.update_type,
                         Message.ScanUpdateType.IR)

    def test_wrong_update(self):
        self.assertNotEqual(self.model.update_type,
                            Message.ScanUpdateType.PING)


class TestMoveMessage(TestCase):
    def setUp(self) -> None:
        """Creates dummy data to be used in tests, and the associated model."""
        self.in_data = dict(direction=Message.MovementDirection.FORWARD,
                            magnitude=5)
        self.model = Message.MoveMessage(**self.in_data)

    def test_direction(self):
        self.assertEqual(self.model.direction,
                         Message.MovementDirection.FORWARD)
