import socket


def establish_socket():
    """
    Create a socket to communicate with the CyBot.
    """
    HOST = '192.168.1.1'
    PORT = 288
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    print(f'Connected to {HOST}:{PORT}')
    return s


sock = establish_socket()
try:
    while True:
        sock.sendall(b"w")
        data = sock.recv(1024)
        print(f"Received - {data.decode('utf-8')}", end='')
except KeyboardInterrupt:
    print("Closing socket!")
    sock.close()
