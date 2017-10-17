// http://www.mikroblog.net/arduino/arduino-code/tm1637-display-example.php
// lib used: https://github.com/avishorp/TM1637
// Same code compiles/works fine for : Digispark, Uno R3, ATTiny85 (yet to test for others like Nano, Pro mini etc.)

#include "TM1637Display.h"

// Uno Pin2 - Data
// Uno Pin3 - CLK 
// for digispark, Pin mapping is from actual number X to PBX, (X doesnot refer to attiny85's  physical pin based on IC pin numbering)

const int CLK = 4; //Set the CLK pin connection to the display  //here 4 indicates PB4 on digispark
const int DIO = 3; //Set the DIO pin connection to the display  //     3 indicates PB3 on digispark
 
int numCounter = 0;
 
TM1637Display display(CLK, DIO); //set up the 4-Digit Display.
 
void setup()
{
  display.setBrightness(0x0a); //set the diplay to maximum brightness
}
 
 
void loop()
{
  for(numCounter = 0; numCounter < 1000; numCounter++) //Iterate numCounter
  {
    display.showNumberDec(numCounter); //Display the numCounter value;
    delay(200);
  }
}
