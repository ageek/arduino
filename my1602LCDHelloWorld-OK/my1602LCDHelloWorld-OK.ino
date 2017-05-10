//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  
  // Print a message to the LCD.
  lcd.backlight();
  // setCursor(ColumnNo from 0 to 1, Row no from 0 to 1)
  //lcd.setCursor(0,0);
  //lcd.print("kick.ass@mail.com");
  //lcd.setCursor(1,1);
  //lcd.print("Hadapsar, PUNE");

  // wait for 2 secs before starting L/R scroll
  delay(2000);

}


void loop()
{
    // set the cursor to (0,0):
  lcd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(500);
  }

  // set the cursor to (16,1):
  lcd.setCursor(16, 1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(500);
  }
}

//// Scroll left and right
////https://www.arduino.cc/en/Tutorial/LiquidCrystalScroll
//void loop() {
//  // scroll 13 positions (string length) to the left
//  // to move it offscreen left:
//  for (int positionCounter = 0; positionCounter < 32; positionCounter++) {
//    // scroll one position left:
//    lcd.scrollDisplayLeft();
//    // wait a bit:
//    delay(250);
//  }
//
//  // scroll 29 positions (string length + display length) to the right
//  // to move it offscreen right:
//  for (int positionCounter = 0; positionCounter < 32; positionCounter++) {
//    // scroll one position right:
//    lcd.scrollDisplayRight();
//    // wait a bit:
//    delay(250);
//  }
//
//  // scroll 16 positions (display length + string length) to the left
//  // to move it back to center:
//  //for (int positionCounter = 0; positionCounter < 32; positionCounter++) {
//    // scroll one position left:
//  //  lcd.scrollDisplayLeft();
//    // wait a bit:
//  //  delay(250);
//  //}
//
//  // delay at the end of the full loop:
//  delay(1000);
//
//}
