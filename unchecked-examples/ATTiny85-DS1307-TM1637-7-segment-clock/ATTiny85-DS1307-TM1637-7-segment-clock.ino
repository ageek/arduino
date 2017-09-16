/*                         Digital Clock
     Binary Clock with RTC & 7 Segments Display - TM1637 (4 Digits x 7 Segments)
                  by LAGSILVA - V4.0 - 24.Jun.2016
*/

#include "TM1637Display.h"              //Display TM1637 library
#include <DS1307RTC.h>                  //DS1307RTC (Real-Time Clock Library) - Pins to ATTINY85: #5 (SDA), #7 (SCK/SCL)
#include <TinyWireM.h>                  //Library of Wire to support DS1307RTC (Real-Time Clock) - Pins to ATTINY85: #5 (SDA), #7 (SCK/SCL)
#include <Time.h>

#define DS1307_I2C_ADDRESS 0x68         // This is the I2C address (RTC)

// Module connection pins (Digital Pins)
#define CLK 3          //ATTINY85 conection on Pin #2  = CLK of Display Module
#define DIO 4          //ATTINY85 conection on Pin #3  = DIO of Display Module

//
//      A
//     ----
//  F |    | B
//     - G-
//  E |    | C
//     ----
//      D
//

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

};

int hh, ht, hu, mm, mt, mu, ss, k1;
unsigned long ti;
byte clockMode;

TM1637Display display(CLK, DIO);


void setup() {

  display.setBrightness (0x0a);//(0x0f) is the max brightness;

  setSyncProvider(RTC.get);   // Update the time with data of RTC (Real Time Clock)
  setSyncInterval(3600);      // Set the number of seconds between re-sync

  TinyWireM.begin();                    // initialize I2C lib

  // Read datum of Clock Mode (Decimal or Binary)stored in RTC
  TinyWireM.beginTransmission(DS1307_I2C_ADDRESS);               // Open I2C line in write mode
  TinyWireM.write((byte)0x08);                                   // Set the register pointer to (0x08)
  TinyWireM.endTransmission();                                   // End Write Transmission
  TinyWireM.requestFrom(DS1307_I2C_ADDRESS, 1);                  // In this case read only 1 byte
  clockMode = TinyWireM.read();                                  // Read the Mode of Clock (Decimal=0 / Binary Traditional =1 / Binary with Seconds = 2) stored at RTC memory

  clockMode = (clockMode + 1) % 3;

  // Write data of Clock Mode (Decimal or Binary) in RTC
  TinyWireM.beginTransmission(DS1307_I2C_ADDRESS);               // Open I2C line in write mode
  TinyWireM.write((byte)0x08);                                   // Set the register pointer to (0x08)
  TinyWireM.write(clockMode);                                    // Record at RTC memory the Mode of Clock
  TinyWireM.endTransmission();                                   // End Write Transmission


  //Clear all digits of display
  display.setSegments(data + 10, 1, 0);                          // Clear digit #1
  display.setSegments(data + 10, 1, 1);                          // Clear digit #2
  display.setSegments(data + 10, 1, 2);                          // Clear digit #3
  display.setSegments(data + 10, 1, 3);                          // Clear digit #4


  //setTime(14, 35, 0, 24, 6, 2016);          // Initial time & date setup of RTC
  //RTC.set(now());                           // Update the RTC with current time

}


void loop() {

  hh = hour();
  mm = minute();
  ss = second();

  switch (clockMode) {

    case 0: //DECIMAL CLOCK

      display.setSegments(data + (hh / 10),  1, 0);              //Display 1 (Hours - Ten)
      display.setSegments(data + (hh % 10),  1, 1);              //Display 2 (Hours - Unit)
      display.showNumberDec(mm, true, 2, 2);                     //Display 3 & 4 (Hours)

      break;


    case 1:  //BINARY CLOCK (Traditional Mode - Without Seconds)

      ht = hh / 10;         //Houts - Tens
      hu = hh % 10;         //Hours - Units
      mt = mm / 10;         //Minutes - Tens
      mu = mm % 10;         //Minutes - Units

      // Plot Hours - Tens

      for (k1 = 0; k1 <= 7; k1++) {
        if (bitRead(ht, k1) == 1) {
          bitWrite(data[0], k1 + 1, 1);
        }
        else {
          bitWrite(data[0], k1 + 1, 0);
        }
      }
      if (ht == 0) {
        bitWrite(data[0], 0, 1);
      }
      else {
        bitWrite(data[0], 0, 0);
      }
      display.setSegments(data + 0, 1, 0);    //Display 1


      // Plot Hours - Units

      for (k1 = 0; k1 <= 7; k1++) {
        if (bitRead(hu, k1) == 1) {
          bitWrite(data[0], k1 + 1, 1);
        }
        else {
          bitWrite(data[0], k1 + 1, 0);
        }
      }
      if (hu == 0) {
        bitWrite(data[0], 0, 1);
      }
      else {
        bitWrite(data[0], 0, 0);
      }
      bitWrite(data[0], 7, 1);
      display.setSegments(data + 0, 1, 1);    //Display 2


      // Plot Minutes - Tens
      for (k1 = 0; k1 <= 7; k1++) {
        if (bitRead(mt, k1) == 1) {
          bitWrite(data[1], k1 + 1, 1);
        }
        else {
          bitWrite(data[1], k1 + 1, 0);
        }
      }
      if (mt == 0) {
        bitWrite(data[1], 0, 1);
      }
      else {
        bitWrite(data[1], 0, 0);
      }
      display.setSegments(data + 1 , 1, 2);   //Display 3


      // Plot Minutes - Units
      for (k1 = 0; k1 <= 7; k1++) {
        if (bitRead(mu, k1) == 1) {
          bitWrite(data[2], k1 + 1, 1);
        }
        else {
          bitWrite(data[2], k1 + 1, 0);
        }
      }
      if (mu == 0) {
        bitWrite(data[2], 0, 1);
      }
      else {
        bitWrite(data[2], 0, 0);
      }
      display.setSegments(data + 2 , 1, 3);   //Display 4

      break;


    case 2:  //BINARY CLOCK (With Seconds)

      // Plot Hours (hh)
      for (int k1 = 0; k1 <= 7; k1++) {
        if (bitRead(hh, k1) == 1) {
          bitWrite(data[0], k1 + 1, 1);
        }
        else {
          bitWrite(data[0], k1 + 1, 0);
        }
      }
      if (hh == 0) {
        bitWrite(data[0], 0, 1);
      }
      else {
        bitWrite(data[0], 0, 0);
      }
      display.setSegments(data + 0, 1, 1);    //Display 2


      // Plot Minutes (mm)
      for (int k1 = 0; k1 <= 7; k1++) {
        if (bitRead(mm, k1) == 1) {
          bitWrite(data[1], k1 + 1, 1);
        }
        else {
          bitWrite(data[1], k1 + 1, 0);
        }
      }
      if (mm == 0) {
        bitWrite(data[1], 0, 1);
      }
      else {
        bitWrite(data[1], 0, 0);
      }
      display.setSegments(data + 1 , 1, 2);   //Display 3


      // Plot Seconds (ss)
      for (int k1 = 0; k1 <= 7; k1++) {
        if (bitRead(ss, k1) == 1) {
          bitWrite(data[2], k1 + 1, 1);
        }
        else {
          bitWrite(data[2], k1 + 1, 0);
        }
      }
      if (ss == 0) {
        bitWrite(data[2], 0, 1);
      }
      else {
        bitWrite(data[2], 0, 0);
      }
      display.setSegments(data + 2 , 1, 3);   //Display 4

      break;

  }

}

