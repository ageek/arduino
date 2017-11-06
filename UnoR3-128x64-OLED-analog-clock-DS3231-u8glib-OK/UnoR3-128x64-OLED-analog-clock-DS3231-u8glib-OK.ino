// Displays a dynamic analog clock --- Shows the date,time etc set via the setcommand
// FIXED:DS3231 stores and display correct time
// BOM: Arduino (pref Uno), DS3231RTC module, 0.96inch SSD1306 OLED display
// 
/******************************************************************************
OLEd Analog Clock using U8GLIB Library
visit https://code.google.com/p/u8glib/ for full details of the U8GLIB library and
full instructions for use.
by Chris Rouse Oct 2014
This version uses a smoother font (profont15) and allows the centre of the clock
to be positioned on the screen by altering the variables clockCentreX and clockCentreY
Note:: Sketch uses 58% of program storage space, 
       Global variables use 48% of dyamic memory, 
       leaving 1,057 bytes for local variables.
Using a IIC 128x64 OLED with SSD1306 chip
RTC DS3231
******************************************************************************/

// Add libraries
#include "U8glib.h"
#include <Wire.h>


#define DS3231_I2C_ADDRESS 0x68

// setup u8g object
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 
         

char monthString[37]= {"JanFebMarAprMayJunJulAugSepOctNovDec"};
int  monthIndex[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
String thisMonth = "";
String thisTime = "";
String thisDay="";
int clockCentreX = 64; // used to fix the centre the analog clock
int clockCentreY = 32; // used to fix the centre the analog clock

//
//TMP36 Pin Variables
// boolean useTMP36 = true;  // set this to false if you don not use TMP36
boolean useTMP36 = false;
#define aref_voltage 3.3 // we tie 3.3V to ARef and measure it with a multimeter!
int tempPin = 1; //the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures
int tempReading; // the analog reading from the sensor
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
  thisTime=String((int)hour) + ":";
  if ((int)minute < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String((int)minute) + ":";
  if ((int)second < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String((int)second);
  const char* newTime = (const char*) thisTime.c_str();
  u8g.drawStr(2,10, newTime);  

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
   String thisTemp1 = String(GetTemp()) + "C";
   // printing output as follows used less program storage space
   const char* thisTemp = (const char*) thisTemp1.c_str();
   // u8glib API ref: https://github.com/olikraus/u8glib/wiki/userreference 
   u8g.drawStr(85,10,thisTemp); 

   // OR - use DS3231's internal temp sensor which is quite accurate
}

void setup(void) {
  Wire.begin(); //for ds3231
  Serial.begin(9600);
  // If you want to set the aref to something other than 5v
  //  analogReference(EXTERNAL);
  //  Wire.begin();
  //  RTC.begin();
  //  if (! RTC.isrunning()) {
  //    Serial.println("RTC is NOT running!");
  //    // following line sets the RTC to the date & time this sketch was compiled
  //    RTC.adjust(DateTime(__DATE__, __TIME__));
  //  }

  // initial date and time information 
  // After the information, comment the line below 
  // (seconds, minutes, hour, day of the week, day of the month, month, year) 
  //myRTC.setDS1302Time(18, 52, 19, 6, 26, 8, 2017);

}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  //delay(1000);
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



  //********for internal temperature sensor
  // https://playground.arduino.cc/Main/InternalTemperatureSensor
double GetTemp(void)
{
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

