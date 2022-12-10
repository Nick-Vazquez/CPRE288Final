"""Implements a basic socket receiver like the one used in the GUI app.
For use if the GUI app shits the bed before demo

__created__ = 2022/12/01
__author__ = Nick Vazquez (nmv)
"""
import enum
import logging
import queue
import sys
import threading

from Models.CyBotMessage import CyBotMessage
from Services import CommunicationService
from Services.CyBotMessageService import CyBotMessageService
from Services.SerialService import SerialService


class TestModes(enum.Enum):
    """Defines the types of input the receiver is expecting."""
    STRING = enum.auto(),
    JSON = enum.auto()


TEST_MODE = TestModes.JSON

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    sock: CommunicationService = SerialService()
    incoming_message_queue: queue.Queue = queue.Queue()

    try:
        logging.info('Generated new serial service.')
        sock.establish_connection()
        threading.Thread(target=sock.setup_poll,
                         args=(incoming_message_queue,),
                         daemon=True).start()
        logging.info('Connection established!')
    except ConnectionRefusedError:
        logging.warning("Could not establish a connection to the CyBot!")
        sys.exit(1)

    message: CyBotMessageService = CyBotMessageService(incoming_message_queue)
    threading.Thread(target=message.start)

    while True:
        try:
            try:
                msg: CyBotMessage = incoming_message_queue.get_nowait()
                print(msg)
            except queue.Empty:
                pass
        except TimeoutError:
            print("No message.")
        except KeyboardInterrupt:
            break

    logging.info('Ending program...')
