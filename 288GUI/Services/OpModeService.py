"""Communication service with particular implementation for requesting and
receiving opmode information.

__created__ = 2022/12/06
__author__ = Nick Vazquez (nmv)
"""
import logging

from Models.CyBotMessage import OpModeMessage, OpModes
from Services import CommunicationService

logger = logging.getLogger(__name__)


class OpModeService:
    """Sends/Receives opmode messages via the given communication provider."""
    def __init__(self, comm_srv: CommunicationService):
        self.comm_srv = comm_srv

    def teleop(self):
        """Commands the CyBot to enter into driver-controlled mode."""
        logger.debug("Teleop")
        message = OpModeMessage(op_mode=OpModes.TELEOP)
        self.comm_srv.send_str(message.json())

    def auto(self):
        """Commands the CyBot to enter into autonomous mode."""
        logger.debug("Auto")
        message = OpModeMessage(op_mode=OpModes.AUTONOMOUS)
        self.comm_srv.send_str(message.json())
