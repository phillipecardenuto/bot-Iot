// Import required libraries
#include <ESP8266WiFi.h>
// WiFi parameters
const char* ssid = "iotic";
const char* password = "iotic2016";
// Host
const char* host = "www.example.com";
//const char* host = "www.timeapi.org";
void setup() {
  
// Start Serial
Serial.begin(115200);
delay(10);
// We start by connecting to a WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
Serial.print("MAC: ");
Serial.println(WiFi.macAddress());
Serial.print("Subnet ");
Serial.println(WiFi.subnetMask());
Serial.print("Gateway ");
Serial.println(WiFi.gatewayIP());
Serial.print("DNS: ");
Serial.println(WiFi.dnsIP());
Serial.print("Channel ");
Serial.println(WiFi.channel());
Serial.print("Status ");
Serial.println(WiFi.status());

}

void loop() {

Serial.print("Connecting to ");
Serial.println(host);
// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;
if (!client.connect(host, httpPort)) {
Serial.println("connection failed");
delay (5000);
return;
}
// This will send the request to the server
client.print(String("GET /") + " HTTP/1.1\r\n" +
"Host: " + host + "\r\n" +
"Connection: close\r\n\r\n");
delay(10);
Serial.println("Read all the lines from server and print them to Serial");
String webpage="";
while(client.available()){
  String line = client.readStringUntil('\r');
  webpage += line;
}    
Serial.println(webpage);
Serial.println("closing connection");
delay(5000);
}
