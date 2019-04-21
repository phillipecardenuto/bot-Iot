# Echo client UDP program
# Sends a datagram to server and receives echo from server
# Repeats every 4 seconds

from time import sleep
import time
import sys
import socket

#HOST = 'localhost'        # The remote host
HOST = '143.106.16.163'        # The remote host
PORT = 50002              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # IP4, datagram socket object
print ('client will send 8 datagrams to server')
ct=0
echofile=sys.argv[1]
tic = time.time()
with open(echofile, 'r') as f: #  abre para leitura
      print("Client will send file to server:", echofile)
      while(1):           # 
        line=f.readline() # le uma linha
        if (not line):    # se fim do arquivo,
            break          #  sai do laco
        ct+=1
        s.sendto(line.encode(), (HOST, PORT))
        #time.sleep(1)
        data, server_addr = s.recvfrom(1024)
        print(data.decode())
toc = time.time()
print ("\nSending a NULL message to simulate 'close connection'")
print("Number of lines",ct)
print("TASK DONE IN %f"%(toc-tic))
s.sendto(''.encode(), (HOST, PORT))
    
