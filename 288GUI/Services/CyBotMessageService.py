"""Defines publisher/subscriber models for communicating between CyBot and
GUI application.

__created__ = 2022/11/28
__author__ = Nick Vazquez (nmv)
"""
import abc
import logging
import queue
import threading
import typing

import Models.CyBotMessage as Message

from Services import CommunicationService

logger = logging.getLogger(__name__)


class _AbstractSubscriber(abc.ABC):
    """Abstract subscriber in a Pub/Sub model."""

    @property
    @abc.abstractmethod
    def accepts(self):
        """Defines the messages this subscriber is willing to accept."""
        pass

    @accepts.setter
    @abc.abstractmethod
    def accepts(self, value: type):
        pass

    @property
    @abc.abstractmethod
    def input_queue(self):
        """Queue to monitor for incoming messages from a publisher."""
        pass

    @abc.abstractmethod
    def poll_input_queue(self):
        """Checks the input queue for any updated messages."""
        pass


class Subscriber(_AbstractSubscriber):
    """Implementation of a subscriber in a pub/sub model."""
    _input_queue: queue.Queue

    def __init__(self):
        self._input_queue = queue.Queue()
        self._accepts: type = Message.CyBotMessage

    @property
    def accepts(self):
        """Defines the messages this subscriber is willing to accept."""
        return self._accepts

    @accepts.setter
    def accepts(self, value: type):
        if not isinstance(value, type):
            raise ValueError("Incorrect accepts type passed."
                             "Needs to be a type!")
        self._accepts = value

    @property
    def input_queue(self):
        """Queue to monitor for incoming messages from a publisher."""
        return self._input_queue

    def poll_input_queue(self):
        """Checks the input queue for any updated messages."""
        pass


class Publisher:
    """Implements a publisher in a Pub/Sub model."""
    def __init__(self):
        self.subscribers: typing.List[Subscriber] = []

    def subscribe(self, subscriber: Subscriber):
        """Adds a subscriber for the publisher to send messages to."""
        if not isinstance(subscriber, Subscriber):
            raise ValueError("Cannot subscribe a non-subscriber object!")
        self.subscribers.append(subscriber)

    def unsubscribe(self, subscriber: Subscriber):
        """Removes a subscriber from receiving messages from this publisher."""
        if subscriber in self.subscribers:
            self.subscribers.remove(subscriber)
        else:
            logger.warning("Tried to remove a non-subscriber from "
                           "subscriptions!")

    def publish(self, message):
        """Outputs the given message to all subscribers."""
        for subscriber in self.subscribers:
            if not isinstance(message, subscriber.accepts):
                # Skip the subscriber if it doesn't accept the type of message.
                continue
            subscriber.input_queue.put(message)


def start_thread(function, *args):
    """Ass function to try to get around threading issues in tkinter."""
    threading.Thread(target=function, args=args, daemon=True).start()


class CyBotMessageService(Publisher):
    """Implementation of a publisher to handle translating incoming messages
    to CyBot messages interpretable by the GUI."""
    def __init__(self, input_q: queue.Queue):
        super().__init__()
        self.logger = logging.getLogger(__class__.__name__)
        self.input_q = input_q

    def start(self):
        """Ass function for getting around tkinter threading issues."""
        start_thread(self.translate_print_recv_messages, self.input_q)

    @staticmethod
    def translate(message_input: dict) -> Message.CyBotMessage:
        """Translates the given message from a dict to a CyBotMessage."""
        cls = Message.message_mapping[message_input.get("mes_type")]
        if cls == Message.ScanResultsMessage:
            message_input['mes_type'] = int(message_input.get('mes_type'))
            message_input['angles'] = [int(val) for val in message_input.get(
                'angles')]
        return cls(**message_input)

    @staticmethod
    def send_msg(msg: Message.CyBotMessage,
                 comm_srv: CommunicationService):
        """Sends a CyBotMessage to the given communication provider.
        Serialization is mostly done through the provider."""
        comm_srv.send_dict(msg.__dict__)

    def translate_print_recv_messages(self, input_q: queue.Queue):
        """Long-running loop to continuously check for incoming messages."""
        while True:
            try:
                msg = input_q.get_nowait()
                if not msg:
                    continue
                translated = self.translate(msg)
                self.publish(translated)
            except queue.Empty:
                pass
            except Exception as e:
                logger.warning(f"Caught an error! {e}")
