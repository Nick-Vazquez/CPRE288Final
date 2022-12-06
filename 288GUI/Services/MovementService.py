import logging

from Services import CommunicationService
from Models.CyBotMessage import MoveMessage, MovementDirection


DEFAULT_TRANSLATION_MAGNITUDE_IN = 100
DEFAULT_ROTATION_MAGNITUDE_DEG = 20

logger = logging.getLogger(__name__)


class MovementService:
    def __init__(self, comm_srv: CommunicationService):
        self.communication = comm_srv

    def forward(self):
        logger.debug("FORWARD")
        message = MoveMessage(direction=MovementDirection.FORWARD,
                              magnitude=DEFAULT_TRANSLATION_MAGNITUDE_IN)
        self.communication.send_str(message.json())

    def reverse(self):
        logger.debug("REVERSE")
        message = MoveMessage(direction=MovementDirection.REVERSE,
                              magnitude=DEFAULT_TRANSLATION_MAGNITUDE_IN)
        self.communication.send_str(message.json())

    def c_clockwise(self):
        logger.debug("COUNTER CLOCKWISE")
        message = MoveMessage(direction=MovementDirection.C_CLOCKWISE,
                              magnitude=DEFAULT_ROTATION_MAGNITUDE_DEG)
        self.communication.send_str(message.json())

    def clockwise(self):
        logger.debug("CLOCKWISE")
        message = MoveMessage(direction=MovementDirection.CLOCKWISE,
                              magnitude=DEFAULT_ROTATION_MAGNITUDE_DEG)
        self.communication.send_str(message.json())
