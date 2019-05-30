//Coloque os símbolos de maior que e menor que como no vídeo! Pois eles não são permitidos aqui. 

//Código alteração da GFX:
#ifdef __AVR__
  #include avr/pgmspace.h
#elif defined(ESP8266) || defined(ESP32)
  #include pgmspace.h
#endif


//O Código utilizado é:

#include Adafruit_GFX.h
#include Adafruit_PCD8544.h

// Software SPI (slower updates, more flexible pin options):
// pin D8 - Serial clock out (SCLK)
// pin D7 - Serial data out (DIN)
// pin D6 - Data/Command select (D/C)
// pin D5 - LCD chip select (CS)
// pin D2 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(D8, D7, D6, D5, D2);
void setup() {
  // put your setup code here, to run once:
   display.begin();
   display.setContrast(50);
   display.clearDisplay();
   display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
