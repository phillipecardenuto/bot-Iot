#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int incomingByte, x, y;
String input = "";

// Create a set of new characters
byte smiley[8] = {
  B00000,  
  B01010,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

byte heart_h[8] = {
  B00000,
  B00000,
  B01010,
  B10101,
  B10001,
  B01010,
  B00100,  
  B00000
}; 
byte heart_f[8] = {
  B00000,
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,  
  B00000
}; 
byte ohm[8] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01010,  
  B11011
}; 
byte rect[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
byte arrow_u[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,  
  B00100
}; 
byte arrow_d[8] = {  
  B00100,
  B00100,
  B00100,
  B00100,  
  B00100,  
  B11111,
  B01110,
  B00100
}; 
byte diamond[8] = {
  B00000,
  B00100,
  B01010,
  B10001,  
  B01010,
  B00100,
  B00000,
  B00000  
}; 
byte JChar[8] = {
  B11111,
  B11111,
  B00010,
  B00010,
  B00010,
  B11010,
  B11110,
  B11110
};
byte Atilde[8] = {
  B01010,
  B10100,
  B00000,
  B01110,
  B01010,
  B01110,
  B01010,
  B01011
};
byte Ochar[8] = {
  B00000,
  B00000,
  B01100,
  B10010,
  B10010,
  B10010,
  B10010,
  B01100
};
byte Telegram[] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B11111,
  B00111,
  B01001,
  B00000
};
int z=1;

void setup(){
  
  lcd.begin(16,2);   // Initialize LCD 2x16


  lcd.createChar (0, heart_h);    // load  heart_h to memory 1
  lcd.createChar (1, heart_f);  // load heart_f to memory 2
  lcd.createChar (2, smiley);    // load smiley to memory 0
//  lcd.createChar (3, ohm);     // load  ohm to memory 3
 lcd.createChar (3, Telegram);     // load  ohm to memory 3
lcd.createChar (4, rect);     // load rect to memory 4
//  lcd.createChar (5, arrow_u);     // load arrow_u to memory 5
//  lcd.createChar (6, arrow_d);     // load arrow_d to memory 6
//  lcd.createChar (7, diamond);     // load diamond to memory 7
  lcd.createChar (5, JChar);     // load JChar to memory 7
  lcd.createChar (6, Atilde);     // load Atilde to memory 8
  lcd.createChar (7, Ochar);     // load Ochar to memory 9

lcd.clear();         //clear the screen
  //lcd.backlight(); // turn on back light
//  lcd.setCursor(0,0); //set cursor to upper left corner
//
//lcd.print(char(0));     //display a smiley
// lcd.print(char(1));    //display a  hollow heart
// lcd.print(char(2));    //display a  filled heart
// lcd.print(char(3));   //display a ohm
// lcd.print(char(4)); //display a rectangle
// lcd.print(char(5)); //display a arrow
// lcd.print(char(6)); //display a rectangle
// lcd.print(char(7)); //display a arrow
//lcd.print(char(8))/*J*/;lcd.print(char(9))/*a*/;lcd.print(char(10));/**/
//lcd.print(char(8))/*J*/;lcd.print(char(9))/*a*/;lcd.print(char(10));/**/

lcd.setCursor(5,1); //set cursor to bottom left second line


lcd.print(char(3));
}
 
void loop() {
lcd.clear();
lcd.setCursor(5,1);
lcd.print(char(5))/*J*/;lcd.print(char(6))/*a*/;lcd.print(char(7));/**/
lcd.print(char(5))/*J*/;lcd.print(char(6))/*a*/;lcd.print(char(7));/**/
lcd.setCursor(0,0);
lcd.print(char(1)); 
lcd.setCursor(15,0);
lcd.print(char(0)); 
lcd.setCursor(1,0);
lcd.print(char(3)); 
delay(400);
for (int tcursor=2;tcursor<=13;tcursor++){
  lcd.setCursor(tcursor-1,0);
  lcd.print(" ");
  lcd.setCursor(tcursor,0);
  lcd.print(char(3)); 
   delay(200); 
}
lcd.setCursor(13,0);
lcd.print(" ");
lcd.setCursor(14,0);
lcd.print(char(3)); 
lcd.setCursor(15,0);
lcd.print(char(1)); 
delay(400);
 
}
