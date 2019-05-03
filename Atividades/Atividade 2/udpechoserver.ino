#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
/* udpechoserver.ino
 *  q
 */


const char* ssid = "Betel";
const char* password = "itapoan8";
WiFiUDP Udp;
unsigned int localUdpPort = 50007;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
//char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back
bool firstpacket=1;
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Udp.begin(localUdpPort);//Initializes the WiFi UDP library and network settings.                               //Starts WiFiUDP socket, listening at local port PORT 
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}
void loop()
{
  int packetSize = Udp.parsePacket();//Checks for the presence of a UDP packet, and reports the size.                                     //parsePacket() must be called before reading the buffer with UDP.read().
 if (packetSize) {    // receive incoming UDP packets   
     if (firstpacket){
         firstpacket=0;
         Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
     }
    int len = Udp.read(incomingPacket, 255);//Reads UDP data from the specified buffer.
    if (len == 0){    //Never enters here!
       Serial.println("Client closed connection sending an empty packet");
       firstpacket=1;
       return;
    }
    incomingPacket[len] = 0;  // C string terminator, needed for printf
    Serial.print(incomingPacket);  // packet should have a lf
    // send back a reply, to the IP address and port we got the packet from
    // beginPacket() and endPacket(). beginPacket() initializes the packet of data, 
    //it is not sent until endPacket() is called.
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());//Starts a connection to write UDP data to the remote connection
    Udp.write(incomingPacket);   //Writes UDP data to the remote connection. Must be wrapped between
    Udp.endPacket();
   }
}
