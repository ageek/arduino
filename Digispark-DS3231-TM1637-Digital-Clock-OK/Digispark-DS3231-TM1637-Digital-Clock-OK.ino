// DS3231 RTC + TM1637 4-digit display for digital clock using Digispark
// Tested on Digispark....should work on other modules e.g. ATTiny85, Uno R3, ProMini, Nano etc

//Libs used: 
//  Using library Time at version 1.5 in folder: C:\PoratableApps\arduino-1.8.1-windows\arduino-1.8.1\portable\sketchbook\libraries\Time 
//  Using library Wire in folder: C:\PoratableApps\arduino-1.8.1-windows\arduino-1.8.1\portable\packages\digistump\hardware\avr\1.6.7\libraries\Wire (legacy)

// v1: display digital clock
// v2: display digital clock + alternate to Temp (TODO)

#include "TM1637Display.h"              //Display TM1637 library
#include "DS1307RTC.h"                  //DS1307RTC (Real-Time Clock Library) - Pins to Arduino UNO: #A4 (SDA), #A5 (SCK/SCL)
#include "Time.h"

// Address is same for DS1307 and DS3231
#define DS1307_I2C_ADDRESS 0x68         // This is the I2C address (RTC)

// Module connection pins (Digital Pins)
#define CLK 4          //Arduino UNO conection on Pin #4  = CLK of Display Module
#define DIO 3          //Arduino UNO conection on Pin #3  = DIO of Display Module

// Display Segments:
//      A
//     ----
//  F |    | B
//     - G-
//  E |    | C
//     ----
//      D
//

// Value of each segment to convert Binary to Decimal
// (Add all corresponding numbers of segments "ON")
//
//      0
//     ----
// 16 |    | 1
//     -32-
//  8 |    | 2
//     ----
//      4
//


uint8_t data[] = {      // Data with colon

  0b10111111, // 0
  0b10000110, // 1
  0b11011011, // 2
  0b11001111, // 3
  0b11100110, // 4
  0b11101101, // 5
  0b11111101, // 6
  0b10000111, // 7
  0b11111111, // 8
  0b11101111, // 9
  0b00000000, // 10
  0b01111001,    // E

};

int hh, mm, ss;
TM1637Display display(CLK, DIO);


void setup() {

  display.setBrightness (0x0a);//(0x0f) is the max brightness;

  //Clear all digits of display
  display.setSegments(data + 10, 1, 0);                          // Clear digit #1
  display.setSegments(data + 10, 1, 1);                          // Clear digit #2
  display.setSegments(data + 10, 1, 2);                          // Clear digit #3
  display.setSegments(data + 10, 1, 3);                          // Clear digit #4
}


void loop() {
  // https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
  tmElements_t tm;

  if (RTC.read(tm)) {
    hh = tm.Hour;
    mm = tm.Minute;
    ss = tm.Second;

    display.setSegments(data + (hh / 10),  1, 0);              //Display 1 (Hours - Ten)
    display.setSegments(data + (hh % 10),  1, 1);              //Display 2 (Hours - Unit)
    display.showNumberDec(mm, true, 2, 2);                     //Display 3 & 4 (Hours)
  } else {
    display.setSegments(data + 11, 1, 0);                          // Error 'E'
  }
}


