//#include <OneWire.h>
#include "LCD5110_Basic.h"
#include "InternalTemperatureSensor.h"
//#include <SoftwareSerial.h>  //pin2 already blocked by 5110 LCD, also disabled for using same code for digispark kit

// for digispark
//Sketch uses 5156 bytes (85%) of program storage space. Maximum is 6012 bytes.
//Global variables use 130 bytes of dynamic memory.

//for attiny85
//Sketch uses 4396 bytes (53%) of program storage space. Maximum is 8192 bytes.
//Global variables use 124 bytes (24%) of dynamic memory, leaving 388 bytes for local variables. Maximum is 512 bytes.


// refer this for pinout: http://2.bp.blogspot.com/-gSqbotFHKfk/UberFXuAOBI/AAAAAAAAAZo/4IUnODX2KuE/s1600/attiny85_pins.png
// pin3 --> physical pin 2  , using standard IC pin numbering
// pin4 --> physical pin 3   

// define Rx, Tx pins
// Rx of tiny should connect to -->  Tx of USB TTL
// Tx of tiny -------------------->  Rx of USB TTL
//SoftwareSerial TinySerial(3, 4); // define pins RX, TX of ATtiny85  as pin3(=physical pin 2), pin4(=physical pin 3) respectively
// prob conflicts with pi2,3 of attiny being used for 5110 ...


//#define ONEWIRE_BUSS 4 
//OneWire TemperatureSensor(ONEWIRE_BUSS); 

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

// The 1.0 argument is ignored by default, see the c’tor comments
InternalTemperatureSensor temperature( 1.0, TEMPERATURE_ADJUSTMENT ); 

void setup(void) {
  
   myGLCD.InitLCD();
   myGLCD.clrScr();

   // Open serial communications and let us know we are connected - pin2 blocked by 5110 and we dont have any spare pins to use
   //TinySerial.begin(9600);
   //TinySerial.println("Tiny Serial Connected via SoftwareSerial Library");

   temperature.init(); // Call init() in setup or every time after you modify ADCSRA or ADMUX
}

void loop(void) {
  
	myGLCD.setFont(BigNumbers);
 
    float t;
    t = (float) temperature.in_c();   // This returns more usable temperature in degrees Celsius

    //myGLCD.printNumF(29.8, 1, CENTER, 14);
    myGLCD.printNumF(t, 1, CENTER, 14);

    // for debugging only - display temp value to soft serial out
    //TinySerial.print("Temperature= ");
    //TinySerial.println(t);

    delay(1000);
}

//Though this method seems to work, using this to display temp somehow prints nothing/junk on 5110 lcd, 
//hence using the internaltempsensor lib with a rolling avg temp

//float getTemp()       
//{
//   // Lets use ATTiny85 internal temperature sensor
//   
//  // https://playground.arduino.cc/Main/InternalTemperatureSensor
//  //double GetTemp(void)
//
//  unsigned int wADC;
//  double t;
//
//  // The internal temperature has to be used
//  // with the internal reference of 1.1V.
//  // Channel 8 can not be selected with
//  // the analogRead function yet.
//
//  // Set the internal reference and mux.
//  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
//  ADCSRA |= _BV(ADEN);  // enable the ADC
//
//  delay(20);            // wait for voltages to become stable.
//
//  ADCSRA |= _BV(ADSC);  // Start the ADC
//
//  // Detect end-of-conversion
//  while (bit_is_set(ADCSRA,ADSC));
//
//  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
//  wADC = ADCW;
//
//  // The offset of 324.31 could be wrong. It is just an indication.
//  t = (wADC - 324.31 ) / 1.22;
//
//  // The returned temperature is in degrees Celsius.
//  return (t);
//}
