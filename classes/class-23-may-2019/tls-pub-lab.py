# coding=utf-8
""" A simple TLS example connecting to test.mosquitto.org
Certificate obtained at www.test.mosquitto.org
connect() return codes are:
0: Connection successful
1: Connection refused â€“ incorrect protocol version
2: Connection refused â€“ invalid client identifier
3: Connection refused â€“ server unavailable
4: Connection refused â€“ bad username or password
5: Connection refused â€“ not authorised
6-255: Currently unused.
***************Tested OK with python script tls-sub-fromesp.py*****************
"""

import paho.mqtt.client as mqtt

#client = mqtt.Mosquitto()
client = mqtt.Client()
client.tls_set("mosquitto-lab.crt") # Certificate File
res=client.connect("xaveco.lab.ic.unicamp.br", 8883, 10) # port 8883 = TLS connection
print "connection result=", res
#------------------------------------------------------------------
def on_publish(client,userdata,result):             #create function for callback
    print("data published :",userdata, result)
client.on_publish = on_publish
#------------------------------------------------------------------
client.loop()
for i in range(5):
    result, mid = client.publish('test', 'hello'+str(i), 1,True) #qos 1 Retain True
    print i,
    if result >0:
        break
print "disconecting..."
client.disconnect()
del client

