import json
import socket
from time import sleep

HOST = "192.168.1.1"
PORT = 288

# HOST = "127.0.0.1"
# PORT = 9998

socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while 1:
    # msg = {"mes_type": 1, "update_type": 1,
    #        "angles": [i for i in range(20)],
    #        "distances": [random.random() * 100 for _ in range(20)]}
    msg = "w"
    socket.sendto(bytes(json.dumps(msg), encoding="UTF-8"), (HOST, PORT))
    print(f'sent {msg} to CyBOT Socket')
    sleep(1)
