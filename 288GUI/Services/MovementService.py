"""
Defines movement commands and interfaces with a given communication service.
Gives default magnitude for CyBot Movements.

__created__ = 2022/12/03
__author__ = Nick Vazquez (nmv)
"""
import logging

from Services import CommunicationService
from Models.CyBotMessage import MoveMessage, MovementDirection


DEFAULT_TRANSLATION_MAGNITUDE_IN = 100
DEFAULT_ROTATION_MAGNITUDE_DEG = 20

logger = logging.getLogger(__name__)


class MovementService:
    """Interfaces between controllers and communication services.
    Instructs the bot on what to do particular to movement."""
    def __init__(self, comm_srv: CommunicationService):
        self.communication = comm_srv

    def forward(self):
        """Moves the bot forward."""
        logger.debug("FORWARD")
        message = MoveMessage(direction=MovementDirection.FORWARD,
                              magnitude=DEFAULT_TRANSLATION_MAGNITUDE_IN)
        self.communication.send_str(message.json())

    def reverse(self):
        """Moves the bot in reverse."""
        logger.debug("REVERSE")
        message = MoveMessage(direction=MovementDirection.REVERSE,
                              magnitude=DEFAULT_TRANSLATION_MAGNITUDE_IN)
        self.communication.send_str(message.json())

    def c_clockwise(self):
        """Moves the bot counter-clockwise."""
        logger.debug("COUNTER CLOCKWISE")
        message = MoveMessage(direction=MovementDirection.C_CLOCKWISE,
                              magnitude=DEFAULT_ROTATION_MAGNITUDE_DEG)
        self.communication.send_str(message.json())

    def clockwise(self):
        """Moves the bot clockwise."""
        logger.debug("CLOCKWISE")
        message = MoveMessage(direction=MovementDirection.CLOCKWISE,
                              magnitude=DEFAULT_ROTATION_MAGNITUDE_DEG)
        self.communication.send_str(message.json())
