/*
  Software serial multple serial test- adapted to work with Digispark
 
 Receives an integer from the software serial, blinks an LED that number of times, and returns a confirmation.
  
This code has been modified for use on an ATtiny.
Created by Matthew on June 11, 2013
http://projectsfromtech.blogspot.com/
 
 This example code is in the public domain.
 
 */
#include "SoftwareSerial.h"

// refer this for pinout: http://2.bp.blogspot.com/-gSqbotFHKfk/UberFXuAOBI/AAAAAAAAAZo/4IUnODX2KuE/s1600/attiny85_pins.png
// PB3/pin3 --> physical pin 2  , using standard IC pin numbering of ATTiny85
// PB4/pin4 --> physical pin 3   

// define Rx, Tx pins
// Rx of USB TTL should connect to -->  Tx of ATtiny85 PB4 ------> P4 of digispark 
// Tx of USB TTL should connect to -->  Rx of ATtiny85 PB3 ------> P3 of digispark
// define pins Rx=PB3, Tx=PB4 of digispark
SoftwareSerial TinySerial(3, 4);   // TinySerial(Rx,Tx)

void setup()  
{
  // Open serial communications and let us know we are connected
  TinySerial.begin(9600);
  TinySerial.println("Tiny Serial Connected via SoftwareSerial Library");
  pinMode(1, OUTPUT);
}

void loop()
{
  if(TinySerial.available())
  {
    // disabled , as compiling for digispark thows error: parseInt() not supported 
//  int received = TinySerial.parseInt();   //Contrary to what the Arduino playground would imply, parseIt is supported
    int received = 10;
    TinySerial.print("Received data...");
    TinySerial.println("Flashing LED");
    if(received >=15)    //So it doesn't blink 150 times when you type 150 instead of 15
      received = 15;     
    Blink(1, received);    //Blink the LED on pin 1
    
  }
}

void Blink(byte led, byte times){ // poor man's display
  for (byte i=0; i< times; i++){
    digitalWrite(led,HIGH);
    // hackish LED flash
    TinySerial.println("---LED---"); // consider this as soft LED flashing/displaying on serial for 'times'
    delay (250);
    digitalWrite(led,LOW);
    delay (175);
  }
}

