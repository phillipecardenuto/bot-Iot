/* mosquitto.ino
 *  connects to a mosquitto server either on local machine or 
 *  an external broker such as xaveco.lab.ic.unicamp.br or 
 *  test.mosquitto.org subscribes to topic "luminpsity" registering
 *  a callback function mycallback and varies luminosity from 0 to MAX
 *  turning user led ON  and OFF
 *  MC853 March 2019
 */

#define LEDP 16

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C"{
#include "user_interface.h"
}

#define ssid          "aula-ic3"
#define password      "iotic@2019"
#define MAX 32         // max luminosity

#define SERVER        "xaveco.lab.ic.unicamp.br" // free MQTT broker
//#define SERVER      "192.168.0.12"        // my PC LAN address
#define SERVERPORT      1883

#define T_LUMINOSITY      "luminosity"
float luminosity, prevLumiosity = 0;  
char valueStr[5];
WiFiClient WiFiClient;
// create MQTT object
PubSubClient client(WiFiClient);
void setup() {
  pinMode(LEDP, OUTPUT);
  Serial.begin(115200);
  delay(10);
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
  WiFi.printDiag(Serial);
  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback); //called when luminosity is published
 }

void loop() {
  yield();
  if (!client.connected()) { // Attempt to connect
    Serial.println("Attempting MQTT connection...");
    if (client.connect("TestMQTT")) { // need choose any name as clientid
      Serial.print("connected to Mosquitto broker at: ");
      Serial.println(SERVER);
      client.subscribe(T_LUMINOSITY, 1);// ... and resubscribe
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 4 seconds");
      // Wait 4 seconds before retrying
      delay(4000);
    }
  }
   delay(500);
   luminosity += 1.0;
   if (luminosity == MAX) luminosity=0;
   if (client.connected() && prevLumiosity != luminosity) {//Publish new luminosity");
    String hi = (String)luminosity;
    hi.toCharArray(valueStr, 5); //only char data can be published
    client.publish( T_LUMINOSITY, valueStr);
    prevLumiosity = luminosity;
   }
   client.loop(); //**** This is essential! *******
 }
//----------------------------------------------------------------------
void callback(char* topic, byte * data, unsigned int length) {
  Serial.print(topic);
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();
  if (luminosity < MAX/2)  {
    digitalWrite(LEDP, LOW);  // led on: led anode to Vcc, catode to P5
    Serial.println("Turning led ON");
  } else {
   digitalWrite(LEDP, HIGH);
    Serial.println("Turning led OFF");  
  }
}
