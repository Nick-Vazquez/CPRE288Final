"""Socket server that mimics a CyBot that's set to send scanResult messages."""
import random
import socket
import time

from Models.CyBotMessage import BayResultsMessage

HOST = "127.0.0.1"
PORT = 65432

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    while True:
        s.listen()
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            while True:
                try:
                    bay = random.randint(1, 4)
                    occ = random.randint(0, 10)
                    message = BayResultsMessage(bay_num=bay, occupancy=occ,
                                                closed=False)
                    byte_data = message.json().encode()
                    conn.sendall(byte_data)
                    print(f"Sent: {byte_data}")
                    time.sleep(3)
                except BrokenPipeError as e:
                    print("Broken pipe... Resetting.")
                    break
