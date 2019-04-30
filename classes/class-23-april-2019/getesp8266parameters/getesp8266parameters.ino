
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#define Show(string, val) Serial.print(string); Serial.println(val);
//const char * ssid="Betel";
//const char * password= "itapoan8";
const char* ssid = "aula-ic3";
const char* password = "iotic@2019";
void scan_all(){
    int n = WiFi.scanNetworks(false, true);
    String wlanssid;
    uint8_t encryptionType;
    int32_t RSSI;
    uint8_t* BSSID;
    int32_t channel;
    bool isHidden;
    int i;
    for (i = 0; i < n; i++)
    {
        WiFi.getNetworkInfo(i, wlanssid, encryptionType, RSSI, BSSID, channel, isHidden);
        Serial.printf("%d: %s, Ch:%d (%ddBm) %s %s\n", i + 1, wlanssid.c_str(), channel, RSSI, encryptionType == ENC_TYPE_NONE ? "open" : "", isHidden ? "hidden" : "");
    }
}
void setup()
{
    WiFi.begin(ssid,password);
    Serial.begin(115200);
    Serial.println();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    Show("AP MAC: ",WiFi.softAPmacAddress());
    Show("Mac Address= ", WiFi.macAddress());
    Show("gateway IP= ", WiFi.gatewayIP());
    Show("subnet Mask= ",WiFi.subnetMask());
    Show("RSSI signal strenght= ", WiFi.RSSI());
    Serial.println("Scanning near by WiFi networks");
    scan_all();
    delay(500);
}

void loop() {

    delay(10000);

}
