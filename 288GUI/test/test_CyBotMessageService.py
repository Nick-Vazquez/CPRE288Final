"""
Tests for the GUI communication service to the CyBot.

__created__ = 2022/11/28
__author__ = Nick Vazquez (nmv)
"""
import json
import queue
from unittest import TestCase
from Services.CyBotMessageService import CyBotMessageService as Service
from Models.CyBotMessage import *


class TestCyBotMessageService(TestCase):
    def setUp(self) -> None:
        """Creates some dummy data to test against."""
        self.in_data = dict(mes_type=4, update_type=ScanUpdateType.IR,
                            angles=[5, 10], distances=[20, 25])
        self.queue = queue.Queue()
        self.sut = Service(self.queue)

    def test_translate_is_cybot_message(self):
        out = self.sut.translate(self.in_data)
        self.assertIsInstance(out, CyBotMessage)

    def test_translate_is_scan_message(self):
        out = self.sut.translate(self.in_data)
        self.assertIsInstance(out, ScanResultsMessage)

    def test_translate_not_move_message(self):
        out = self.sut.translate(self.in_data)
        self.assertNotIsInstance(out, MoveMessage)

    def test_from_json(self):
        print(json.dumps(self.in_data))
