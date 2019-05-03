/*echoserver.ino - echoes lines sent by echoclient.py
 * iterative server connects on port 50007
 * use echoclient-tcp.py to send lines of a text file to server
 * 
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#define Show(string,val) Serial.print(string); Serial.println(val); // string= cadeia/valor, val= variÃ¡vel cujo valor queremos exibir


#define Show(string,val)Serial.print(string);Serial.println(val); //macro for common print case
WiFiUDP Udp;
unsigned int localUdpPort = 50007;
unsigned int serverPort = 50007;
const char* serverIP = "177.220.84.49";
const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
char outPackage[255];
char req[255];

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
        memset(outPackage,'\0',sizeof(outPackage));
        sprintf(outPackage,"Hello Word %d \n",ct);
        Serial.print("SEND:");Serial.println(outPackage);

        //Right Way sending the message until it arrives back
        while(!Udp.parsePacket()){
          Udp.beginPacket(serverIP, serverPort);
          Udp.write(outPackage);
          Udp.endPacket();
          delay(500);
        };
        
        memset(req,'\0',sizeof(outPackage));
        Udp.read(req,255);
        Serial.print("Received: ");Serial.println(req);
    }
    Serial.println("Finished!");
    delay(1000);
}
