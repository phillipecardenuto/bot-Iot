/*echoserver.ino - echoes lines sent by echoclient.py
 * iterative server connects on port 50007
 * use echoclient-tcp.py to send lines of a text file to server
 * 
 */
#include <ESP8266WiFi.h>
#define Show(string,val) Serial.print(string); Serial.println(val); // string= cadeia/valor, val= variÃ¡vel cujo valor queremos exibir


#define Show(string,val)Serial.print(string);Serial.println(val); //macro for common print case
const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
const char* server = "177.220.84.49";

//WiFiServer server(50007); // Create an instance of the server
WiFiClient client;        // and one of client to talk to echoclient
//*****************************************************************
void setup() {
    Serial.begin(921600);
    delay(10);
    Serial.println();
    Show("Echoserver is connecting to: ",ssid);
    WiFi.begin(ssid, password);    //WiFi connect
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
   // server.begin();        // Start the server
    Serial.println("Server started");
    Show("local IP= ",WiFi.localIP());
    
    Serial.println("Connected to wifi!");
    Serial.println("Sending mensage to server");
    if (client.connect(server,50007)) {
      Serial.println("Connected to Server!");
      client.setTimeout(0);              // to cancel implicit 1 sec timeout
      int ct = 0;
      while (1){
        ct+=1;
        if (ct >= 8) break;
        String outstring = String(ct);
        outstring = String("Hello Word " + outstring +"\n");
        Serial.println(outstring);
        client.print(outstring);
      }
     Serial.println("Finished!");
    } 
}
//**********************************************************
void loop() {
  
}
