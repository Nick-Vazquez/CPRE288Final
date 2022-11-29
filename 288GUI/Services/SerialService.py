import logging
import socket
import json
import time
import traceback

HOST = '192.168.1.1'
PORT = 288


def set_host_and_port(host: str, port: int):
    global HOST, PORT
    HOST = host
    PORT = port


class SerialService:
    connection_timeout_s: int
    connection: socket.socket

    def __init__(self):
        self.connection_timeout_s: int = 10
        self.logger = logging.getLogger(str(__class__.__name__))

    def establish_socket(self) -> bool:
        self.connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.logger.info(f'Attempting to connect... '
                             f'({self.connection_timeout_s}s)')
            self.connection.settimeout(self.connection_timeout_s)
            self.connection.connect((HOST, PORT))
            self.logger.info(f'Connected to {HOST}:{PORT}')
            return True
        except TimeoutError:
            self.logger.critical("Couldn't establish connection! Timed out."
                                 f"Stack: {traceback.format_exc()}")
            return False

    def reconnect(self):
        self.connection.close()
        self.connection.connect((HOST, PORT))

    def __del__(self):
        self.logger.debug("Closing...")
        if hasattr(self, 'connection'):
            self.logger.debug("Closed connection!")
            self.connection.close()

    def send_str(self, data: str):
        # Hopefully we won't need to use connect here and can keep the
        # connection alive for the whole time the instance is alive
        # self.connection.connect((HOST, PORT))
        try:
            sent_bytes = self.connection.sendall(bytes(data, encoding="utf-8"))
            return sent_bytes
        except TimeoutError:
            self.logger.critical("Timeout error when connecting!\n"
                                 + traceback.format_exc())
            return 0

    def send_json(self, data: dict):
        as_json = json.dumps(data)
        self.send_str(as_json)

    def get_message(self) -> str:
        data = self.connection.recv(1024)
        decoded = data.decode('utf-8')
        self.logger.debug(f"Received - {decoded}")
        return decoded

    def serialize_json(self, message: str):
        return json.loads(message)

    def get_json(self) -> dict:
        return self.serialize_json(self.get_message())


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    sock = SerialService()
    logging.info('Generated new service.')
    sock.establish_socket()
    while True:
        try:
            user = input('Enter direction key...')
            if user not in ('w', 'a', 's', 'd'):
                logging.warning("Incorrect key pressed!")
                continue
            counter = 500
            while counter > 0:
                sent = sock.send_str(user)
                time.sleep(0.001)
                if sent == 0:
                    sock.reconnect()
                counter -= 1
        except KeyboardInterrupt:
            break

    logging.info('Ending program...')