import logging
import queue
import threading

import Models.CyBotMessage as Message

from Services import CommunicationService


def start_thread(function, *args):
    threading.Thread(target=function, args=args, daemon=True).start()


class CyBotMessageService:
    def __init__(self, input_q: queue.Queue):
        self.logger = logging.getLogger(__class__.__name__)
        self.input_q = input_q

    def start(self):
        start_thread(self.translate_print_recv_messages, self.input_q)

    @staticmethod
    def translate(message_input: dict) -> Message.CyBotMessage:
        cls = Message.message_mapping[message_input.get("mes_type")]
        return cls(**message_input)

    def send_msg(self, msg: Message.CyBotMessage,
                 comm_srv: CommunicationService):
        comm_srv.send_dict(msg.__dict__)

    def translate_print_recv_messages(self, input_q: queue.Queue):
        while True:
            try:
                msg = input_q.get_nowait()
                if not msg:
                    continue
                translated = self.translate(msg)
                self.logger.info(translated.json())
            except queue.Empty:
                pass
