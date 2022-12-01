import logging
import socket
import select

HOST = "192.168.1.1"
PORT = 288

# HOST = "127.0.0.1"
# PORT = 9998

socket_1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
socket_1.bind((HOST, PORT))
poller = select.poll()
poller.register(socket_1, select.POLLIN)

try:
    while 1:
        events = poller.poll(0.1)
        for sock, evt in events:
            if evt and select.POLLIN:
                msg = socket_1.recvfrom(4096)
                print('received poll event from socket')
                print(f"Message: {msg}")
except KeyboardInterrupt:
    pass

logging.info("Ending program...")
