from unittest import TestCase

import Models.CyBotMessage
import Models.CyBotMessage as Message


class TestScanMessage(TestCase):
    def setUp(self) -> None:
        self.in_data = dict(update_type=Message.ScanUpdateType.IR,
                            angles=[120], distances=[59.5])
        self.model = Models.CyBotMessage.ScanMessage(**self.in_data)

    def test_msg_type(self):
        # Assert
        self.assertEqual(self.model.mes_type, 1)

    def test_update_type(self):
        self.assertEqual(self.model.update_type,
                         Message.ScanUpdateType.IR)

    def test_wrong_update(self):
        self.assertNotEqual(self.model.update_type,
                            Message.ScanUpdateType.PING)


class TestMoveMessage(TestCase):
    def setUp(self) -> None:
        self.in_data = dict(direction=Message.MovementDirection.FORWARD,
                            magnitude=5)
        self.model = Message.MoveMessage(**self.in_data)

    def test_direction(self):
        self.assertEqual(self.model.direction,
                         Message.MovementDirection.FORWARD)
