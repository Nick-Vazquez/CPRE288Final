"""Communication service with particular implementation for requesting and
receiving scan information.

__created__ = 2022/12/03
__author__ = Nick Vazquez (nmv)
"""
import logging

from Models.CyBotMessage import RequestScanMessage, ScanUpdateType
from Services import CommunicationService

logger = logging.getLogger(__name__)


# noinspection GrazieInspection
class ScanService:
    """Sends/Receives scan messages via the given communication provider."""
    def __init__(self, comm_srv: CommunicationService):
        self.communication = comm_srv

    def request_scan_ir(self):
        """Requests an updated IR scan from the Bot"""
        logger.debug("Requesting IR scan from CyBot...")
        message = RequestScanMessage(update_type=ScanUpdateType.IR, full=True)
        self.communication.send_str(message.json())

    def request_scan_ping(self):
        """Requests an updated Ping scan from the Bot"""
        logger.debug("Requesting ping scan from CyBot...")
        message = RequestScanMessage(update_type=ScanUpdateType.PING, full=True)
        self.communication.send_str(message.json())
