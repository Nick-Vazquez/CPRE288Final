import abc
import json


class CommunicationService(abc.ABC):
    @abc.abstractmethod
    def establish_connection(self):
        """Connects to the classes' communication provider"""
        pass

    @abc.abstractmethod
    def reconnect(self):
        """Reconnects to the communication provider.
        Used for cases where connections hang."""
        pass

    @abc.abstractmethod
    def send_str(self, data: str):
        """Sends a string message over the communication provider."""
        pass

    @abc.abstractmethod
    def get_str(self):
        """Gets a string message over the communication provider."""
        pass

    def send_dict(self, data: dict):
        as_json = json.dumps(data)
        self.send_str(as_json)

    def get_json(self):
        """Gets a JSON message over the communication provider."""
        return self._to_json(self.get_str())

    @staticmethod
    def _to_json(message: str):
        """Converts a string message to a JSON entity."""
        return json.loads(message)
