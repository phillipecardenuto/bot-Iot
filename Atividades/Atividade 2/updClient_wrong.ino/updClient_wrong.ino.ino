/*echoserver.ino - echoes lines sent by echoclient.py
 * iterative server connects on port 50007
 * use echoclient-tcp.py to send lines of a text file to server
 * 
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define Show(string,val) Serial.print(string); Serial.println(val); // string= cadeia/valor, val= variÃ¡vel cujo valor queremos exibir


#define Show(string,val)Serial.print(string);Serial.println(val); //macro for common print case
WiFiUDP Udp;
unsigned int localUdpPort = 50007;
unsigned int serverPort = 50007;
const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
const char* server = "177.220.84.49";
bool firstpacket=1;

//WiFiServer server(50007); // Create an instance of the server
WiFiClient client;        // and one of client to talk to echoclient
//*****************************************************************
void setup() {
    Serial.begin(921600);
    delay(10);
    Serial.println();
    Show("Echoserver-UDP is connecting to: ",ssid);
    WiFi.begin(ssid, password);    //WiFi connect
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Udp.begin(localUdpPort);
    Serial.println("UDP Connection Done!");
    Show("local IP= ",WiFi.localIP());
    Serial.println("Connected to wifi!");
   
}
//**********************************************************
void loop() {
   int ct=0;
   while (1){ 
        ct+=1;
        if (ct >= 8) break;
        String outstring = String(ct);
        outstring = String("Hello Word " + outstring +"\n");
        Udp.beginPacket(server, serverPort);
        Udp.print(outstring);
        Serial.print("SEND:");Serial.println(outstring);
        
        
        int packsize = Udp.parsePacket();
        //If the message get lost, this while will wait forever, because we are using UDP!!
        while(packsize==0) {
          packsize = Udp.parsePacket();
          Serial.println(packsize);
        }
        String req = Udp.readString();
        delay(500);
        Serial.print("Received: ");
        Serial.println(req);
        Udp.endPacket();
      }
    
    Serial.println("Finished!");
}
