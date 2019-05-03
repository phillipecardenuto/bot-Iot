/*echoserver.ino - echoes lines sent by echoclient.py
 * iterative server connects on port 50007
 * use echoclient-tcp.py to send lines of a text file to server
 * 
*/
#include <ESP8266WiFi.h>

#define Show(string,val)Serial.print(string);Serial.println(val); //macro for common print case
const char* ssid = "Betel";     // WiFi parameters
const char* password = "itapoan8";

WiFiServer server(50007); // Create an instance of the server
WiFiClient client;        // and one of client to talk to echoclient
//*****************************************************************
void setup() {
  Serial.begin(115200);
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
  server.begin();        // Start the server
  Serial.println("Server started");
  Show("local IP= ",WiFi.localIP());
}
//**********************************************************
void loop() {
  client = server.available();  // Check if a client has connected
  if (!client) 
    return;                     // no, start loop() again
  Serial.println("new client");
   while (!client.available()){    
       delay(1);                    
    }
  while (1){                        //loop will read and echo lines sent by client
    if (!client.connected())break;  
    client.setTimeout(0);              // to cancel implicit 1 sec timeout
    String req = client.readString(); //Until('\n'); //Until('\n');  //in this call
    Serial.print(req); 
    //client.flush(); // this is essential!
    client.print(req);
  }             // while loop ends here
  Serial.println("Client disconnected");
}
