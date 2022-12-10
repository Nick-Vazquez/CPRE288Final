"""Tests sockets to implement a GUI Continuously sending "Hello, world" to
the CyBot.

__created__ = 2022/12/03
__author__ = Nick Vazquez (nmv)
"""

import socket

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 65432  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    for _ in range(4):
        s.sendall(b"Hello, world")
        data = s.recv(1024)
        print(f"Received {data!r}")

print("Disconnected.")
