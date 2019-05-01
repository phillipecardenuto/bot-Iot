#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
lcd.begin(16, 2);
lcd.print("Funcionou VINA!");
lcd.cursor();

}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  String dis = "Uptime: " ;
  dis = dis + millis()/1000 ;
  // print the number of seconds since reset:
  lcd.print(dis);
}
