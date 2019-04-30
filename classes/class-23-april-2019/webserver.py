# Echo server program
import socket
mypage= """HTTP/1.1 200 OK
Content-Type: text/html


<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css">
</head>
<div class="container">
<h1>Relay Control</h1>
<div class="row">
<div class="col-md-2"><input class="btn btn-block btn-lg btn-primary" type="button" value="On" onclick="on()"></div>
<div class="col-md-2"><input class="btn btn-block btn-lg btn-danger" type="button" value="Off" onclick="off()"></div>
</div></div>
<script>function on() {$.get("/on");}</script>
<script>function off() {$.get("/off");}</script>
"""
startmsg= """
Web Server emulation: receives a HTTP GET request from an emulated browser
and sends back an html page as generated by the sketch lamp_control_progmem.ino
"""
print (startmsg)
HOST = ''                       # Symbolic name meaning all available interfaces
PORT = 50007              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creates socket IP4, tcp
s.bind((HOST, PORT)) #accepts connection from HOST on port PORT
s.listen(1)    # historical reason....
while (1):		# infinite loop
    conn, addr = s.accept()		# blocks until connection occurs
    print ('Connected by', addr)	# shows client IP and client local port
    data = conn.recv(2048)		# 2048 = arbitrary max data accepted
    print ("received this request from browser:\n", data.decode())
    print ("will now send an Html page to browser and close connection to browser")
    conn.send(mypage.encode())
    conn.close()		# close connection and go back to wait for another coonection

