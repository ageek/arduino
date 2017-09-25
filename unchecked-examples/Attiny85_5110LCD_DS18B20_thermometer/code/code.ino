#include <OneWire.h>

#define ONEWIRE_BUSS 4
 
OneWire TemperatureSensor(ONEWIRE_BUSS); 

#include <LCD5110_Basic.h>

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
 
 myGLCD.printNumF(t, 1, CENTER, 14);
 
 
}
