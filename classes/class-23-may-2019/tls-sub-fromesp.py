# coding=utf-8
""" A simple TLS example connecting to test.mosquitto.org
Certificate obtained at www.test.mosquitto.org
connect() return codes are:
0: Connection successful
1: Connection refused – incorrect protocol version
2: Connection refused – invalid client identifier
3: Connection refused – server unavailable
4: Connection refused – bad username or password
5: Connection refused – not authorised
6-255: Currently unused.
tls subscribe sketch on topic test of test.mosquitto.org
esp8266 sends a message to this topic
"""

import paho.mqtt.client as mqtt
#import time
#client = mqtt.Mosquitto()
client = mqtt.Client()
client.tls_set("mosquitto.org.crt") # Certificate File
res=client.connect("test.mosquitto.org", 8883, 10) # port 8883 = TLS connection
print "connection result=", res
pubtopic="foo"
client.subscribe('test')
ct=0
def on_message(client,userdata,msg): # callback for messages published by esp
    global ct
    print ('message received: topic= '+ str(msg.topic)+ " payload= " + msg.payload)
    client.publish(pubtopic, "Hello esp8266-"+str(ct))
    ct=ct+1
client.on_message= on_message

client.loop_forever()   #loop waiting for messages
                        # when message arrives publish another to sender




