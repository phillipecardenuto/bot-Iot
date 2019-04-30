/***************************************************
echoserver.ino
This server echoes lines sent by a client written in python,
on port 50007. An interesting test is the client 
sending a text file like its own program
VERY SLOW! Challenge: make it faster!
MC853 March-June 2019
****************************************************/

#include <ESP8266WiFi.h>
const char* ssid = "Betel";
const char* password = "itapoan8";
// Create an instance of the server; wait fpr connection on port 50007
WiFiServer server(50007);

void setup() { 
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

String req;

void loop() {
  WiFiClient client = server.available();   // Check if a client has connected
  if (!client) {       // yet no client connected
    return;
  }
  while(1){                    // will echo lines until client closes connection
     if (!client.connected())break;
     while(!client.available()){ // Wait until the client sends some data
        delay(1);                // should not occur!
        }                                               
     req = client.readStringUntil('\r');  // Read the first line of the request
     Serial.print(req);     // we expect message with lf
     client.print(req);     // Send the response to the client
     //Serial.flush();   //client.flush();
  }
  Serial.println("Client disconnected\n"); // back to loop, wait for a new connection
  }
