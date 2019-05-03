# Echo server UDP program
# Binds to port 50007 and waits for a datagram on that  port
# Echoes to client received datagram and repeats

import socket
HOST = ''                       # Symbolic name meaning all available interfaces
PORT = 50007              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # IP4, datagram socket object
s.bind((HOST, PORT))
print ('server is waiting for client to send a datagram on port %d:'%PORT)
flag=0        # set to 1 when first message is received
while 1:
    data,addr = s.recvfrom(1024)
    if data:
        if not flag:
            flag=1
            print ("client socket address= ", addr)  # just once
        sent = s.sendto(data, addr)	# echoes received datafram
        print ("received and echoed: ",data)
    else:
        break    # exit loop when client sends a null message
print ("Client sent a NULL message! Server quits!")



