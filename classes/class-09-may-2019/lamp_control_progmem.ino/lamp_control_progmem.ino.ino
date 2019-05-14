/***************************************************
Written by Marco Schwartz for Open Home Automation.  
BSD license, all text above must be included in any redistribution
Based on the original sketches supplied with the ESP8266/Arduino 
implementation written by Ivan Grokhotkov      
****************************************************/

// Required libraries
#include <ESP8266WiFi.h>

// WiFi parameters
const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
const char PROGMEM PAGE_root[] = R"=====(
HTTP/1.1 200 OK
Content-Type: text/html


<head>
<meta http-equiv="refresh" content="2" >
<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css">
</head>
<div class="container">
<h1>Relay Control</h1>
<div class="row">
<div class="col-md-2"><input class="btn btn-block btn-lg btn-primary" type="button" value="Off" onclick="off()"></div>
<div class="col-md-2"><input class="btn btn-block btn-lg btn-danger" type="button" value="On" onclick="on()"></div>
</div></div>
<script>function on() {$.get("/on");}</script>
<script>function off() {$.get("/off");}</script>
)=====";
/*
<meta http-equiv="refresh" content="2">
<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate"/>
<meta http-equiv="Pragma" content="no-cache"/>
<meta http-equiv="Expires" content="0"/>
*/
// Create an instance of the server
WiFiServer server(80);

// Pin
int output_pin = D0;
String s;
void setup() {
  
  // Start Serial
  Serial.begin(115200);
  delay(10);
  pinMode(output_pin, OUTPUT);
  digitalWrite(output_pin, 0);  //test with led  initially On 
  
  // Connect to WiFi network
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
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  // Prepare the response
  s=PAGE_root;   // copies html page to String s
    
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request    // Note: no match when page is loaded!
  if (req.indexOf("/on") != -1){
    digitalWrite(output_pin, 0);
    Serial.println("Pressed ON");
    }
  else if (req.indexOf("/off") != -1) {
    digitalWrite(output_pin, 1);
    Serial.println("Pressed OFF");
    }   
  // Send the response to the client
  client.print(s);
  // delay(10);
 // Serial.println("Client disconnected");

  // The client will actually  disconnect
  // when the function returns and 'client' object is detroyed
}
