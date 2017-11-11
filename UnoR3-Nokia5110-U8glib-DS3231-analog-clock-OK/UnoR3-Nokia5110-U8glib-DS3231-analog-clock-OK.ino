// http://henrysbench.capnfatz.com/henrys-bench/arduino-displays/arduino-nokia-5110-with-u8glib-tutorial/
// Nov 2017: 
//  Modified to use U8glib for analog clock display on 5110 using DS3231

// Add libraries
#include "U8glib.h"
#include <Wire.h>


#define DS3231_I2C_ADDRESS 0x68
#define DS3231_TEMPERATURE_MSB 0x11
#define DS3231_TEMPERATURE_LSB 0x12


// setup u8g object
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 

U8GLIB_PCD8544 u8g(3,4,7,5,6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6
         

char monthString[37]= {"JanFebMarAprMayJunJulAugSepOctNovDec"};
int  monthIndex[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
String thisMonth = "";
String thisTime = "";
String thisDay="";
int clockCentreX = 20; // used to fix the centre the analog clock
int clockCentreY = 24; // used to fix the centre the analog clock

//
 

void draw(void) {
  //
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
  
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_profont15);
  //u8g.setFont(u8g_font_6x10); 
  //
  
  //********* display date at bottom of screen
  thisDay = String((int)dayOfMonth, DEC) + "/"; 
  thisMonth="";
  for (int i=0; i<=2; i++){
    thisMonth += monthString[monthIndex[(int)month-1]+i];
  }  
  thisDay=thisDay + thisMonth + "/"; 
  thisDay=thisDay + String((int)year , DEC);
  const char* newDay = (const char*) thisDay.c_str(); 
  u8g.drawStr(32,63, newDay);   

  
 
  // *********display time in digital format
  thisTime="";
  thisTime=String((int)hour) ;
  thisTime += (int)second %2==0? " " : ":" ;
  if ((int)minute < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String((int)minute) ;
  //if ((int)second < 10){ thisTime=thisTime + "0";} // add leading zero if required
  //thisTime=thisTime + String((int)second);
  const char* newTime = (const char*) thisTime.c_str();
  u8g.drawStr(45,10, newTime);  

  // for testing/debugging purposes only
  Serial.println(newTime);
  
  //
  // ***********Now draw the clock face
  u8g.drawCircle(clockCentreX, clockCentreY, 20); // main outer circle
  u8g.drawCircle(clockCentreX, clockCentreY, 2);  // small inner circle
  //
  
  //hour ticks
  for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    angle=(angle/57.29577951) ; //Convert degrees to radians
    int x2=(clockCentreX+(sin(angle)*20));
    int y2=(clockCentreY-(cos(angle)*20));
    int x3=(clockCentreX+(sin(angle)*(20-5)));
    int y3=(clockCentreY-(cos(angle)*(20-5)));
    u8g.drawLine(x2,y2,x3,y3);
  }
  // display second hand
  float angle = (int)second *6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  int x3=(clockCentreX+(sin(angle)*(20)));
  int y3=(clockCentreY-(cos(angle)*(20)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  
  // display minute hand
  angle = (int)minute * 6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(clockCentreX+(sin(angle)*(20-3)));
  y3=(clockCentreY-(cos(angle)*(20-3)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  
  // display hour hand
  angle = (int)hour * 30 + int(((int)minute / 12) * 6 )   ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(clockCentreX+(sin(angle)*(20-11)));
  y3=(clockCentreY-(cos(angle)*(20-11)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  
  //==========Temperature 

  //*********for now lets try print Temp using Arduino internal temp sensor
//   String thisTemp1 = String(GetTemp()) + "C";
//   // printing output as follows used less program storage space
//   const char* thisTemp = (const char*) thisTemp1.c_str();
//   // u8glib API ref: https://github.com/olikraus/u8glib/wiki/userreference 
//   u8g.drawStr(85,10,thisTemp); 

   // OR - use DS3231's internal temp sensor which is quite accurate
   
   String thisTemp1 = String(DS3231_getTemperature()) + "C";
   // printing output as follows used less program storage space
   const char* thisTemp = (const char*) thisTemp1.c_str();
   // u8glib API ref: https://github.com/olikraus/u8glib/wiki/userreference 
   u8g.drawStr(40,40,thisTemp); 
   
}

void setup(void) {
  Wire.begin(); //for ds3231
  Serial.begin(9600);
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
   
}

///////////////////////////////////////////
// supporting methods for ds3231
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

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
