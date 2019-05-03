/*TCPClient.ino - echoes lines sent by echoclient.py
 * iterative server connects on port 50007
 * use echoclient-tcp.py to send lines of a text file to server
 * 
 */
#include <ESP8266WiFi.h>
#define Show(string,val) Serial.print(string); Serial.println(val);

const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
const char* serverIP = "177.220.84.49";
unsigned int serverPort = 50007;


WiFiClient client;        // and one of client to talk to echoclient
//*****************************************************************
void setup() {
    Serial.begin(921600);
    delay(10);
    Serial.println();
    Show("TCPClient is connecting to: ",ssid);
    WiFi.begin(ssid, password);    //WiFi connect
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("Server started");
    Show("local IP= ",WiFi.localIP());
    Serial.println("Connected to wifi!");
   
}
//**********************************************************
void loop() {
    if (client.connect(serverIP,serverPort)) {
      
      Serial.println("Connected to Server!");
      client.setTimeout(0);              // to cancel implicit 1 sec timeout
      int ct = 0;
      while (1){
        ct+=1;
        if (ct >= 8) break;
        String outstring = String(ct);
        outstring = String("Hello Word " + outstring +"\n");
        client.print(outstring);
        delay(500);
        String req = client.readString();
        Serial.print("Received: ");
        Serial.println(req);
      }
     client.stop();
     Serial.println("Finished!");
     delay(1000);
  }
  Show("local IP= ",WiFi.localIP());
  delay(1000);
}
