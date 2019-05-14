//Initialized variable to store recieved data
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
#define HS_KEY 123
SoftwareSerial uno_serial(0,1);

void serial_hs(){
  int data=0;
  while (data != HS_KEY){
  Serial.print(".");
  data = uno_serial.read();
  data = uno_serial.write(HS_KEY);  
  delay(1000);
  }
}

void setup() {
  //Serial Begin at 9600 Baud 
  Serial.begin(115200);
  Serial.println("SETTING UP ARDUINO | NODEMCU HANDSHAKE");
  uno_serial.begin(9600);
//   serial_hs();
  Serial.println("HANDSHAKE DONE!");
 
  
}

void loop() {
  //data = uno_serial.read();
  String data = uno_serial.readString();
  if(data!= "")
    Serial.println(data); //Read the serial data and store it
  delay(100);
}
