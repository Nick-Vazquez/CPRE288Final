import logging
import queue
import socket
import select
import time
import traceback

from Services import CommunicationService

# HOST = '192.168.1.1'
# PORT = 288

HOST = "127.0.0.1"
PORT = 65432


def set_host_and_port(host: str, port: int):
    global HOST, PORT
    HOST = host
    PORT = port


class SerialService(CommunicationService):
    connection_timeout_s: int
    connection: socket.socket
    poller: select.poll

    def __init__(self, master):
        super().__init__()
        self.connection_timeout_s: int = 5
        self.logger = logging.getLogger(str(__class__.__name__))
        self.master = master

    def establish_connection(self) -> bool:
        self.connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            # self.connection.bind((HOST, PORT))
            self.logger.info(f'Attempting to connect... '
                             f'({self.connection_timeout_s}s)')
            self.connection.settimeout(self.connection_timeout_s)
            self.connection.connect((HOST, PORT))
            self.logger.info(f'Connected to {HOST}:{PORT}')
            self.connected = True

            self.poller = select.poll()
            self.poller.register(self.connection, select.POLLIN)
            return True
        except TimeoutError:
            self.logger.critical("Couldn't establish connection! Timed out."
                                 f"Stack: {traceback.format_exc()}")
            return False

    def poll_incoming_messages(self, output_queue: queue.Queue):
        """NOTE: This should be run in a separate thread."""
        events = self.poller.poll(5000)
        for recv_socket, event in events:
            if event and select.POLLIN:
                if not recv_socket == self.connection.fileno():
                    raise RuntimeError("Received messages from an "
                                       "unexpected socket!")
                recv_json = self.get_json()
                output_queue.put(recv_json)
        self.master.after(100, self.poll_incoming_messages, output_queue)

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
        super(SerialService, self).send_str(data)
        try:
            sent_bytes = self.connection.sendto(
                bytes(data, encoding="utf-8"), (HOST, PORT))
            return sent_bytes
        except TimeoutError:
            self.logger.critical("Timeout error when connecting!\n"
                                 + traceback.format_exc())
            return 0

    # TODO: Check to see if this stops at a \0.
    def get_str(self) -> str:
        super(SerialService, self).get_str()
        data = self.connection.recv(4096)
        decoded = data.decode('utf-8')
        self.logger.debug(f"Received - {decoded}")
        return str(decoded)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    sock = SerialService()
    logging.info('Generated new service.')
    sock.establish_connection()
    while True:
        try:
            user = input('Enter direction key...')
            if user not in ('w', 'a', 's', 'd', '+', '-'):
                logging.warning("Incorrect key pressed!")
                continue
            counter = 20
            while counter > 0:
                sent = sock.send_str(user)
                time.sleep(0.001)
                if sent == 0:
                    sock.reconnect()
                counter -= 1
        except KeyboardInterrupt:
            break

    logging.info('Ending program...')
