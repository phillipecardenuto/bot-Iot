# Echo server program
import socket
import time

HOST = ''                       # Symbolic name meaning all available interfaces
PORT = 50007              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print("Server waiting connection from any host, on port:",PORT)
conn, addr = s.accept() # conn is a new "connection socket"
print ("connected to: ",addr)
while 1:
    data = conn.recv(1024)
    if not data: break
    print("Received:",data.decode())
    conn.send(data)
conn.close()
