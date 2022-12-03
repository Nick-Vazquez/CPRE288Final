import logging

from Services import CommunicationService
from Models.CyBotMessage import RequestScanMessage, ScanResultsMessage, \
    ScanUpdateType

logger = logging.getLogger(__name__)


class ScanService:
    def __init__(self, comm_srv: CommunicationService):
        self.communication = comm_srv

    def request_scan_ir(self):
        logger.debug("Requesting IR scan from CyBot...")
        message = RequestScanMessage(update_type=ScanUpdateType.IR, full=True)
        self.communication.send_str(message.json())

    def request_scan_ping(self):
        logger.debug("Requesting ping scan from CyBot...")
        message = RequestScanMessage(update_type=ScanUpdateType.PING, full=True)
        self.communication.send_str(message.json())
