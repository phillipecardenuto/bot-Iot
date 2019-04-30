#define Show(string,val) Serial.print(string); Serial.println(val);
#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11 
const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
//DHT dht(DHTPIN, DHTTYPE);
//**************************************************************
void setup() {
  Serial.begin(115200);
  delay(10);
  //dht.begin();
  Serial.println();
  Serial.println();
  WiFi.begin(ssid, password);  // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Show("WiFi connected to:",ssid);
  Show("Esp8266 local IP: ",WiFi.localIP());
  server.begin(); // Start the server
  Serial.println("Server started: open browser, enter as URL your local IP");
}
//********************************************************************
void loop() { 
  WiFiClient client = server.available();  // Check if a client has connected
  if (!client) {
    return;
  }
  Serial.println("new client");   
  while(!client.available()){     // Wait until the client sends some data
    delay(1);
  }
  float h = 0;// dht.readHumidity();
  float t = 0 ;// dht.readTemperature();  // Read temperature as Celsius (the default)
  if (isnan(h) || isnan(t)) {    // Check if any reads failed and exit early (to try again).
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  String req = client.readStringUntil('\r'); // Read the first line of the request
  Serial.println(req);
  client.flush();
  if (req.indexOf("/1.1") != -1){
    client.flush();
    client.println("HTTP/1.1 200 OK");     // This tells the browser that the request to provide data was accepted
    client.println("Access-Control-Allow-Origin: *");  //Tells the browser accepted its request from a different domain 
    client.println("Content-Type: application/json;charset=utf-8");//Tell itthat the data will be in a JSON format
    client.println("Server: ESP8266");// The data is coming from an Arduino Web Server (this line can be omitted)
    client.println("Connection: close");         // Will close the connection at the end of data transmission.
    client.println();    // Required: this blank line,tells the browser it has reached the end of header                      
    client.print("{\"temperature\": \"");  // This is the starting bracket of the JSON data
    client.print(t);                           
    client.print("\", \"Humidity\": \"");
    client.print(h);               
    client.print("\"}");                                         
  }
  else {               //invalid request: repeat sending headers as before, then send an invalid response
    client.flush();
    client.println("HTTP/1.1 200 OK");           
    client.println("Access-Control-Allow-Origin: *"); 
    client.println("Content-Type: application/json;charset=utf-8");  //the data will be in a JSON format
    client.println("Server: Arduino");          
    client.println("Connection: close");        
    client.println();                       // This is tha starting bracket of the JSON data 
    client.print("{\"Response\": ");
    client.print("Invalid");                          
    client.print("}");                                        // This is the final bracket of the JSON data
  }
    delay(1);
    Serial.println("Client disonnected");
}
