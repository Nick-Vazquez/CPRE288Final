import enum
import logging

from Services import CommunicationService
from Services.SerialService import SerialService


class TestModes(enum.Enum):
    STRING = enum.auto(),
    JSON = enum.auto()


TEST_MODE = TestModes.JSON

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    sock: CommunicationService = SerialService()
    logging.info('Generated new serial service.')
    sock.establish_connection()
    while True:
        try:
            if TEST_MODE == TestModes.STRING:
                msg = sock.get_str()
            elif TEST_MODE == TestModes.JSON:
                msg = sock.get_json()
            else:
                raise AttributeError("Test mode couldn't be determined!")
            print(msg)
        except TimeoutError:
            print("No message.")
        except KeyboardInterrupt:
            break

    logging.info('Ending program...')
