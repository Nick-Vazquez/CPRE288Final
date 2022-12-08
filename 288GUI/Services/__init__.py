"""
Defines services to interface with external programs and devices from the GUI.

__created__ = 2022/11/29
__author__ = Nick Vazquez (nmv)
"""
import abc
import json
import queue


class CommunicationService(abc.ABC):
    """Abstract communication service with bidirectional data flow."""
    def __init__(self):
        self.connected = False

    def check_connection(self):
        """Ensures there is an active connection before executing
        instructions on the connection."""
        if not self.connected:
            raise ConnectionError('Not connected to a communication endpoint!')

    @abc.abstractmethod
    def establish_connection(self):
        """Connects to the classes' communication provider"""
        pass

    @abc.abstractmethod
    def setup_poll(self, output_queue: queue.Queue):
        """Starts all polling and adds messages to output_queue."""
        pass

    @abc.abstractmethod
    def start_polling_incoming_messages(self, output_queue: queue.Queue):
        """Infinite loop to poll incoming messages over the communication
        provider."""
        pass

    @abc.abstractmethod
    def reconnect(self):
        """Reconnects to the communication provider.
        Used for cases where connections hang."""
        pass

    def send_str(self, data: str):
        """Sends a string message over the communication provider."""
        self.check_connection()

    @abc.abstractmethod
    def get_str(self):
        """Gets a string message over the communication provider."""
        self.check_connection()

    def send_dict(self, data: dict):
        """Sends a string representation of a dictionary over the communication
        provider."""
        self.check_connection()
        as_json = json.dumps(data)
        self.send_str(as_json)

    def get_json(self):
        """Gets a JSON message over the communication provider."""
        self.check_connection()
        return self._to_json(self.get_str())

    @staticmethod
    def _to_json(message: str):
        """Converts a string message to a JSON entity."""
        return json.loads(message)
