/*echoserver.ino - echoes lines sent by echoclient.py
 * iterative server connects on port 50007
 * use echoclient-tcp.py to send lines of a text file to server
 * 
*/
#include <ESP8266WiFi.h>
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);
#define HS_KEY 123


void serial_hs(){
  int data=0;
  while (data != HS_KEY){
  s.write(HS_KEY);
  data = s.read();  
  delay(1000);
  }
}
const char* ssid = "aula-ic3";     // WiFi parameters
const char* password = "iotic@2019";

WiFiServer server(50007); // Create an instance of the server
WiFiClient client;        // and one of client to talk to echoclient
//*****************************************************************
void setup() {
  //Serial S Begin at 115200 Baud
  s.begin(9600);
//  serial_hs();
  delay(100);
  s.print("Echoserver is connecting to: ");s.println(ssid);
  WiFi.begin(ssid, password);    //WiFi connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  s.println("");
  s.println("WiFi connected");
  server.begin();        // Start the server
  s.println("Server started");
  s.print("local IP= ");s.println(WiFi.localIP());
}
//**********************************************************
void loop() {
  client = server.available();  // Check if a client has connected
  if (!client) 
    return;                     // no, start loop() again
  s.println("new client");
   while (!client.available()){    
       delay(1);                    
    }
  while (1){                        //loop will read and echo lines sent by client
    if (!client.connected())break;  
    client.setTimeout(0);              // to cancel implicit 1 sec timeout
    String req = client.readString(); //Until('\n'); //Until('\n');  //in this call
    s.print(req); 
    //client.flush(); // this is essential!
    client.print(req);
  }             // while loop ends here
  s.println("Client disconnected");
}
