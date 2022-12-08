import logging

from Models.CyBotMessage import OpModeMessage, OpModes
from Services import CommunicationService

logger = logging.getLogger(__name__)


class OpModeService:
    def __init__(self, comm_srv: CommunicationService):
        self.comm_srv = comm_srv

    def teleop(self):
        logger.debug("Teleop")
        message = OpModeMessage(op_mode=OpModes.TELEOP)
        self.comm_srv.send_str(message.json())

    def auto(self):
        logger.debug("Auto")
        message = OpModeMessage(op_mode=OpModes.AUTONOMOUS)
        self.comm_srv.send_str(message.json())
