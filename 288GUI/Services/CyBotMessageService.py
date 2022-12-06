import abc
import logging
import queue
import threading
import typing

import Models.CyBotMessage as Message

from Services import CommunicationService

logger = logging.getLogger(__name__)


class _AbstractSubscriber(abc.ABC):
    @property
    @abc.abstractmethod
    def accepts(self):
        pass

    @accepts.setter
    @abc.abstractmethod
    def accepts(self, value: type):
        pass

    @property
    @abc.abstractmethod
    def input_queue(self):
        pass

    @abc.abstractmethod
    def poll_input_queue(self):
        pass


class Subscriber(_AbstractSubscriber):
    _input_queue: queue.Queue

    def __init__(self):
        self._input_queue = queue.Queue()
        self._accepts: type = Message.CyBotMessage

    @property
    def accepts(self):
        return self._accepts

    @accepts.setter
    def accepts(self, value: type):
        if not isinstance(value, type):
            raise ValueError("Incorrect accepts type passed."
                             "Needs to be a type!")
        self._accepts = value

    @property
    def input_queue(self):
        """Queue that receives subscriber messages."""
        return self._input_queue


class Publisher:
    def __init__(self):
        self.subscribers: typing.List[Subscriber] = []

    def subscribe(self, subscriber: Subscriber):
        if not isinstance(subscriber, Subscriber):
            raise ValueError("Cannot subscribe a non-subscriber object!")
        self.subscribers.append(subscriber)

    def unsubscribe(self, subscriber: Subscriber):
        if subscriber in self.subscribers:
            self.subscribers.remove(subscriber)
        else:
            logger.warning("Tried to remove a non-subscriber from "
                           "subscriptions!")

    def publish(self, message):
        for subscriber in self.subscribers:
            if not isinstance(message, subscriber.accepts):
                # Skip the subscriber if it doesn't accept the type of message.
                continue
            subscriber.input_queue.put(message)


def start_thread(function, *args):
    threading.Thread(target=function, args=args, daemon=True).start()


class CyBotMessageService(Publisher):
    def __init__(self, input_q: queue.Queue):
        super().__init__()
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
                self.publish(translated)
            except queue.Empty:
                pass
