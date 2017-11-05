//http://electronicshobbyists.com/interfacing-nokia-5110-lcd-with-arduino-nokia-5110-arduino-tutorial/
//https://github.com/yvest/PCD8544-Nokia-5110-LCD-library/blob/master/PCD8544.cpp
// the default pin connections are like this: 

//Connect the pin 1 (RST Pin) to the pin 6 of Arduino through the 10K resistor.
//Connect the pin 2 (SCE Pin) to the pin 7 of Arduino through the 1K resistor.
//Connect the pin 3 (D/C Pin) to the pin 5 of Arduino through the 10K resistor.
//Connect the pin 4 (DIN Pin) to the pin 4 of Arduino through the 10K resistor.
//Connect the pin 5 (CLK Pin) to the pin 3 of Arduino through the 10K resistor.
//Connect the pin 6 (VCC Pin) to the 3.3V pin of Arduino.
//Connect the pin 7 (LED Pin) to the middle pin of 1k potentiometer through 330 ohm resistor and connect the other two pins to the VCC and the ground.
//Connect the pin 8 (GND Pin) to the GND of Arduino.

#include <PCD8544.h>
 
 
 PCD8544 lcd;

void setup() {
   lcd.begin(84, 48);
   lcd.setContrast(35);   //with Light ON, 35 looks good, 30-50 is OK
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME  ");

  lcd.setCursor(0, 1);
  lcd.print("     To");

  lcd.setCursor(0,2);
  lcd.print("ABCDEFGHIJKLmnopqrstubw!@#$%^&^*() MARIO WORLD");

  delay(200);
 }

