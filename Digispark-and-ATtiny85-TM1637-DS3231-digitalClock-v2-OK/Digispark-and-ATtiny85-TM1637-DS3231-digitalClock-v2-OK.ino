// Tested on digispark kit - should work on attiny85 chip as well
// v2 - display temp as well (say for 2 sec on every 30th sec?)
//TODO:
// v3 - put the attiny85 to sleep (how much to sleep)
// v4 - wake up every X seconds, dispay time and temp and go back to sleep
// final version - most power efficient version that can be run for 5-6 months on say 500maH 18650 battery without any boost converter for 5v

//PB3 --330 OHM LA Vcc
//divizorul 330 Ohm la GND si 680 Ohm la Vcc,

#include <Wire.h>
#include "TM1637.h" 
#define CLK 1       //attiny85 pin PB1         
#define DIO 3       //attiny85 pin PB3   DIO on PB4 is not working(for digispark - no idea about attiny85 bare chip), need to check why???
#define brightness 1 

 int buton;
 
TM1637 tm1637(CLK,DIO);  
#define DS3231_I2C_ADDRESS 0x68
  
volatile boolean flag;
 unsigned char ClockPoint = 1;
byte decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
}
 
byte bcdToDec(byte val){
  return ( (val/16*10) + (val%16) );
}
 
void setDateDs3231(byte second,  
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year)          // 0-99
{
   Wire.beginTransmission(DS3231_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(second));    
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));     
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}
 
void getDateDs3231(byte *second,
          byte *minute,
          byte *hour,
          byte *dayOfWeek,
          byte *dayOfMonth,
          byte *month,
          byte *year)
{
 
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
 
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
 
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f); 
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}
 
void setINT(){  
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x0E);
  Wire.write(0x0);
  Wire.endTransmission();
}
 

 
void setup() {

  Wire.begin();

  tm1637.init();
  tm1637.set(brightness);  

}

void loop(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; 
  getDateDs3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 
  int8_t TimeDisp[4]; 
  
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
 ClockPoint = (~ClockPoint) & 0x01;
 if(ClockPoint)
    tm1637.point(POINT_ON);
 else 
    tm1637.point(POINT_OFF); 
 delay(500);
  
//  buton = analogRead(PB3);
// 
// if (buton>=100 && buton<=700){
//   second = 0;     
//      hour++;  
//      if (hour > 23) hour = 0;  
//      setDateDs3231(second, minute, hour, dayOfWeek, dayOfMonth, month, year); 
//      delay(200);
//  }
//  if (buton>=0 && buton<=50){
//      second = 0;
//      minute++;
//      if (minute > 59) minute = 0;
//      setDateDs3231(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
//      delay(200);
//  }

// set ds3231 time
//      setDateDs3231(10, 35, 22, 2, 1, 10, 2018);    // working fine - uncomment for setting time

     // tm1637.display(TimeDisp);
    // Test: display temp on every 30th sec , for 2 secs 
    if ((int)second % 30 == 0) {
        // disable POINT
        tm1637.point(POINT_OFF);
        // TODO : just print one point and print float temp
        int temp = (int) DS3231_getTemperature() ;
    
     
        int digitoneT = temp / 10;
        int digittwoT = temp % 10;
    
        tm1637.display(0,digitoneT); 
        tm1637.display(1,digittwoT);
        tm1637.display(2,18); // put degree
        tm1637.display(3,12);  // put a C at the end
        
        delay (2000);
    } else {
       tm1637.display(TimeDisp);    
    }

}

//my custom method
//*******************************Taken from Uno DS3231.cpp lib
// Working perfectly for Digispark AND ATtiny85 
float DS3231_getTemperature() {
  // Checks the internal thermometer on the DS3231 and returns the 
  // temperature as a floating-point value.

  // Updated / modified a tiny bit from "Coding Badly" and "Tri-Again"
  // http://forum.arduino.cc/index.php/topic,22301.0.html
  
  byte tMSB, tLSB;
  float temp3231;
  
  // temp registers (11h-12h) get updated automatically every 64s
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);

  // Should I do more "if available" checks here?
  if(Wire.available()) {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion

    temp3231 = ((((short)tMSB << 8) | (short)tLSB) >> 6) / 4.0;
  }
  else {
    temp3231 = -9999; // Some obvious error value
  }
   
  return temp3231;
}


