
   /*
    HTTP over TLS (HTTPS) example sketch

    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
      TLSv1.2 is supported since version 2.4.0-rc1
    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
    -----------------------------------------------------
    Modified by Celio G. for use im Mc853 - March 2019
    Publishes to port 8883 of test.mosquitto.org, topic test
    and subscribes to topic foo. Tested with python script
    http://www.ic.unicamp.br/~celio/mc853/python/tls-sub-fromesp.py
    which subscribes to topic test and publishes to topuc foo
    each time it receives a message in topic test.
    Messages are numbered and shown on Serial Monitor of sketch
    and in python terminal window.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <time.h>

#define Show(string, val) Serial.print(string); Serial.println(val);
#define STASSID "Betel"
#define STAPSK  "itapoan8"
const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "test.mosquitto.org";
const int httpsPort = 8883;
// sha1 of test.mosquitto.org
const char fingerprint[] PROGMEM = "7E 36 22 01 F9 7E 99 2F C5 DB 3D BE AC 48 67 5B 5D 47 94 D2";

char* topic = "test";
String clientName;
long lastReconnectAttempt = 0;
long lastMsg = 0;
int test_para = 2000;
unsigned long startMills;

 // Use WiFiClientSecure class to create TLS connection
   void callback(char* topic, byte* payload, unsigned int length);
   WiFiClientSecure client;
   PubSubClient pubsubclient(host, 8883, callback,client);
 
//****************************************************************************************8i
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("connecting to ");
  Serial.println(host);
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);
  
  pubsubclient= pubsubclient.setServer(host,8883);
  if (!pubsubclient.connected()) {
          //if (!pubsubclient.connect(host, httpsPort)) {
     if (!pubsubclient.connect("mc853")) {  // clientid cant be empty
        Serial.println("connection failed");
        return;
      }
   }
   Show("\n**** Connected on port 8883 of host: ",host);
   if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
   long now = millis();
   if (now - lastMsg > test_para) {
        lastMsg = now;
        String payload = "{\"startMills\":";
        payload += (millis() - startMills);
        payload += "}";
        sendmqttMsg(topic, payload);
      }
    pubsubclient.loop();
    delay(5000);
  Serial.println("closing connection");
}
//**********************************************************************
void sendmqttMsg(char* topictosend, String payload)
{
 
  if (client.connected()) {
      Serial.print("Sending payload: ");
      Serial.print(payload);
    unsigned int msg_length = payload.length();
      Serial.print(" length: ");
      Serial.println(msg_length);
    byte* p = (byte*)malloc(msg_length);
    memcpy(p, (char*) payload.c_str(), msg_length);
    if ( pubsubclient.publish(topictosend, p, msg_length)) {
        Serial.println("Publish ok");
      free(p);
      //return 1;
    } else {
        Serial.println("Publish failed");
      free(p);
      //return 0;
    }
  }
}
//********************************************************************
void callback(char* topic, byte* payload, unsigned int length) {
  char bytearray[length];
  memcpy(bytearray, payload, length);
  bytearray[length]=0;
  Show("Callback topic= ",String(topic));
  Show ("Payload- ", String(bytearray));
  Show ("Payload length- ", length); 
}
//********************************************************************
void loop() {
  yield();
}
