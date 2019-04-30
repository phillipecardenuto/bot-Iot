// Import required libraries
#include <ESP8266WiFi.h>
// WiFi parameters
const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
// Host
const char* host = "http://ident.me/";
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
        setup();
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
        /*
           for (int i=0; i< line.length(); i++)
           scan[i]=line[i];
           pt=scan; 
         */
    }    
    Serial.println(webpage);
    /*l1: while (*pt++ != '\r');
      if (*pt++ == '\n' && *pt++ != '\r')
      goto l1;
      pt++;  // skip linefeed-- desnecessário se só queremos hh.mm.ss!
      while (*pt++ != 'T');
     *(pt+8)='\n';  // essas 2 linhas só para exibir bonitinho no vídeo
     *(pt+9)=0;  
     }
     Serial.print(scan);
     */
    Serial.println("closing connection");
    delay(5000);
    }
