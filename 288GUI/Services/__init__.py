import abc
import json


class CommunicationService(abc.ABC):
    def __init__(self):
        self.connected = False

    def check_connection(self):
        if not self.connected:
            raise ConnectionError('Not connected to a communication endpoint!')

    @abc.abstractmethod
    def establish_connection(self):
        """Connects to the classes' communication provider"""
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
        self.check_connection()
        as_json = json.dumps(data)

    def get_json(self):
        """Gets a JSON message over the communication provider."""
        self.check_connection()
        return self._to_json(self.get_str())

    @staticmethod
    def _to_json(message: str):
        """Converts a string message to a JSON entity."""
        return json.loads(message)
