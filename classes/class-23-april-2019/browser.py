# Echo client program
import socket
MAXDATASIZE= 2048	# parameter max size of server response
GETrequest=""" 
GET / HTTP/1.1
Host: localhost:80

"""
print ("This program emulates a browser requesting an html page from a Web Server")

HOST = 'localhost'        # The remote host DNS address
PORT = 50007              # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creates IP4, tcp socket
s.connect((HOST, PORT))   # starts 3 way connection handshake protocol
print ("Will now send this request to Server:", GETrequest)
s.send(GETrequest.encode())	   # sends an http GET request so server
data = s.recv(MAXDATASIZE) # receives data sent back from server
print ('Received from server:\n', data.decode())
print ("\n\nNote: connection will be closed by server")
