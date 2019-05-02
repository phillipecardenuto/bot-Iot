#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int incomingByte, x, y;
String input = "";
void setup() {
lcd.begin(16, 2);
Serial.begin(9600);
lcd.print("Hello World!");
lcd.cursor();

}
void loop() {
   if (Serial.available()) {
  lcd.clear();
  delay(100);
  while (Serial.available() > 0) {
   input = Serial.readString();
   Serial.println(input.length());
    lcd.print(input);
    delay(400);

   }
   }
     if (input.length() > 15){
    input = input;
    Serial.println("ENTREI");
    for (int positionCounter = 0; positionCounter < input.length(); positionCounter++) {
   // scroll one position left:
   lcd.scrollDisplayLeft();
   // wait a bit:
    delay(100);
  }
    
   }
  
 // to move it offscreen left:
//  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
//    // scroll one position left:
//    lcd.scrollDisplayLeft();
//    // wait a bit:
//    delay(150);
//  }
//
//  // scroll 29 positions (string length + display length) to the right
//  // to move it offscreen right:
//  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
//    // scroll one position right:
//    lcd.scrollDisplayRight();
//    // wait a bit:
//    delay(150);
//  }
//
//  // scroll 16 positions (display length + string length) to the left
//  // to move it back to center:
//  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
//    // scroll one position left:
//    lcd.scrollDisplayLeft();
//    // wait a bit:
//    delay(150);
//  }
//
//  // delay at the end of the full loop:
//  delay(1000);

}
/*
  if (Serial.available() > 0) {
    
    x = x + 1;
    y = y + 1;
    if(x > 15){
     lcd.setCursor(0,2);
     x = 0;
    }
    if(y > 31){
     lcd.setCursor(0,0);
     y = 0;
     x = 0;
    }
  }
}
*/
