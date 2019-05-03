# Echo server program
import socket
import time
HOST = ''                       # Symbolic name meaning all available interfaces
PORT = 50007              # Arbitrary non-privileged port
print("Starting echo server")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
while 1:
    conn, addr = s.accept()
    print ('Connected by', addr)
    while 1:
        data = conn.recv(1024)
        if not data: break
        print(data.decode())
        conn.send(data)
    conn.close()
    time.sleep(10);
