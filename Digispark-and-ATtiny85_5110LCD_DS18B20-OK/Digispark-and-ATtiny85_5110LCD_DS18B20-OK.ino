#include <OneWire.h>
#include "LCD5110_Basic.h"
//#include <SoftwareSerial.h>  //pin2 already blocked by 5110 LCD, also disabled for using same code for digispark kit

//IMP Note:
// DS18b20 should be connected as per standard connections 
// for chineese clones 1->Vcc, 2-data, 3->Gnd , 2->pulled high to 5v via 4.7K resistor (for orig ones 1->gnd, 3->vcc)
// This code will not work for Parasitic power mode  for DS18b20 (1,3->gnd, 2->data and pulled to high 5v via 4.7K)


// refer this for pinout: http://2.bp.blogspot.com/-gSqbotFHKfk/UberFXuAOBI/AAAAAAAAAZo/4IUnODX2KuE/s1600/attiny85_pins.png
// pin3 --> physical pin 2  , using standard IC pin numbering
// pin4 --> physical pin 3   

// define Rx, Tx pins
// Rx of tiny should connect to -->  Tx of USB TTL
// Tx of tiny -------------------->  Rx of USB TTL
//SoftwareSerial TinySerial(3, 4); // define pins RX, TX of ATtiny85  as pin3(=physical pin 2), pin4(=physical pin 3) respectively
// prob conflicts with pi2,3 of attiny being used for 5110 ...


#define ONEWIRE_BUSS 4      //PB4 is 4 (not 3 as inferred by standard IC pin numbering)
OneWire TemperatureSensor(ONEWIRE_BUSS); 

//P0 to P5 of digispark ==> PB0 to PB5 of ATtiny85
//
// Nokia 5110 LCD connections to ATtiny85 pins---------DigisparkPins
// SCK (CLK) --------------- Attiny85 pin 5------------P0
// MOSI (DIN)--------------- Attiny85 pin 6------------P1
// DC (register select) ---- Attiny85 pin 7------------P2
// RST --------------------- Attiny85 pin 2------------P3
// CS (CE) ----------------- GND-----------------------GND

//                     __________ SCK (CLK)
//                    /  ________ MOSI (DIN)
//                   /  /  ______ DC (register select)
//                  /  /  /  ____ RST
//                 /  /  /  /  __ CS (CE)
//                /  /  /  /  /
  LCD5110 myGLCD(0, 1, 2, 3, 6); //D6 don't exist - conect CS to GND

extern uint8_t BigNumbers[];


void setup(void) {
  
   myGLCD.InitLCD();
   myGLCD.clrScr();

   // Open serial communications and let us know we are connected - pin2 blocked by 5110 and we dont have any spare pins to use
   //TinySerial.begin(9600);
   //TinySerial.println("Tiny Serial Connected via SoftwareSerial Library");
}

void loop(void) {
  
 myGLCD.setFont(BigNumbers);
 
    byte i;
    byte data[12];
    int16_t raw;
    float t;
 
    TemperatureSensor.reset();       // reset one wire buss
    TemperatureSensor.skip();        // select only device
    TemperatureSensor.write(0x44);   // start conversion
 
    delay(1000);                     // wait for the conversion
 
    TemperatureSensor.reset();
    TemperatureSensor.skip();
    TemperatureSensor.write(0xBE);   // Read Scratchpad
    
    for ( i = 0; i < 9; i++) {       // 9 bytes
      data[i] = TemperatureSensor.read();
    }
 
    raw = (data[1] << 8) | data[0];
    t = (float)raw / 16.0;

    // for testing only - use ATtiny85's internal temp sensor
    //t = getTemp();
    
    myGLCD.printNumF(t, 1, CENTER, 14);

    // for debugging only - display temp value to soft serial out
    //TinySerial.print("Temperature= ");
    //TinySerial.println(t);
}

char getTemperature()
{
   return random(20,30); 
}

float getTemp() 
{
   // Lets use ATTiny85 internal temperature sensor
   
  // https://playground.arduino.cc/Main/InternalTemperatureSensor
  //double GetTemp(void)

  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celsius.
  return (t);
}
