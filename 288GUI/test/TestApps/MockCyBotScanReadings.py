"""Socket server that mimics a CyBot that's set to send scanResult messages."""
import random
import socket
import time

from Models.CyBotMessage import ScanResultsMessage, ScanUpdateType

HOST = "127.0.0.1"
PORT = 65432

mock_data = [_ for _ in range(90)]

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    while True:
        s.listen()
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            while True:
                try:
                    message = ScanResultsMessage(
                        update_type=ScanUpdateType.IR, angles=mock_data,
                        distances=[random.randint(1, 30) for _ in range(90)])
                    byte_data = message.json().encode()
                    conn.sendall(byte_data)
                    print(f"Sent: {byte_data}")
                    time.sleep(0.25)
                except BrokenPipeError as e:
                    print("Broken pipe... Resetting.")
                    break
