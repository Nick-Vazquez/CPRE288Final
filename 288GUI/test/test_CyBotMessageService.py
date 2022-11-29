from unittest import TestCase
from Services.CyBotMessageService import CyBotMessageService as Service
from Models.CyBotMessage import *


class TestCyBotMessageService(TestCase):
    def setUp(self) -> None:
        self.in_data = dict(mes_type=1, update_type=ScanUpdateType.IR,
                       angles=[5, 10], distances=[20, 25])
        self.sut = Service()

    def test_translate_is_cybot_message(self):
        out = self.sut.translate(self.in_data)
        self.assertIsInstance(out, CyBotMessage)

    def test_translate_is_scan_message(self):
        out = self.sut.translate(self.in_data)
        self.assertIsInstance(out, ScanMessage)

    def test_translate_not_move_message(self):
        out = self.sut.translate(self.in_data)
        self.assertNotIsInstance(out, MoveMessage)
