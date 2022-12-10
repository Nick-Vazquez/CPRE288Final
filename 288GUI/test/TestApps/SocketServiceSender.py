"""Implements a basic socket sender like the one used in the GUI app.
Messages the CyBot from a cmdline runner.
For use if the GUI app shits the bed before demo

__created__ = 2022/12/03
__author__ = Nick Vazquez (nmv)
"""
import logging
import time

from Models.CyBotMessage import RequestScanMessage, ScanUpdateType
from Services import CommunicationService
from Services.SerialService import SerialService

# HOST = "192.168.1.1"
# PORT = 288

HOST = "127.0.0.1"
PORT = 65432

message = RequestScanMessage(update_type=ScanUpdateType.PING, full=True)

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    sock: CommunicationService = SerialService()
    logging.info("Generated new serial service.")
    sock.establish_connection()
    while True:
        sock.send_str(message.json())
        time.sleep(1)
