// displays a dynamic analog clock --- Shows the date,time etc set via the setcommand,....
// Integrated with DS3231 RTC for both real time clock as well as temperature
// BOM: Arduino (pref Uno), DS3231RTC module,0.96inch SSD1306 OLED display
// 
// Orig code edited to add support for DS3231 RTC 
/******************************************************************************
OLEd Analog Clock using U8GLIB Library 

u8glib API ref: https://github.com/olikraus/u8glib/wiki/userreference 
visit https://code.google.com/p/u8glib/ for full details of the U8GLIB library and
full instructions for use.
by Chris Rouse Oct 2014
This version uses a smoother font (profont15) and allows the centre of the clock
to be positioned on the screen by altering the variables clockCentreX and clockCentreY
Note:: Sketch uses 58% of program storage space, 
       Global variables use 48% of dyamic memory, 
       leaving 1,057 bytes for local variables.
Using a IIC 128x64 OLED with SSD1306 chip
Optional Temperature Sensor TMP 36
Wire DS3231 RTC:
  VCC +5v
  GND GND
  SDA Analog pin 4
  SCL Analog pin 5
Wire SSD1306 OLED:
  VCC +5v
  GND GND
  SDA Analog pin 4
  SCL Analog pin 5
  
Connect ARef to 3.3v on Arduino
******************************************************************************/

  // Add libraries
  #include "U8glib.h"
  #include <Wire.h>
  #include "DS3231.h"
 


  // setup u8g object
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 
  
  //disable if you want to use Arduino's internal temperature 
  bool useRTCforTemp=true;
            
  DS3231 myRTC ;
  bool Century=false;
  bool h12;
  bool PM;
  byte ADay, AHour, AMinute, ASecond, ABits;
  bool ADy, A12h, Apm;


  char monthString[37]= {"JanFebMarAprMayJunJulAugSepOctNovDec"};
  int  monthIndex[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
  String thisMonth = "";
  String thisTime = "";
  String thisDay="";
  int clockCentreX = 64; // used to fix the centre the analog clock
  int clockCentreY = 32; // used to fix the centre the analog clock
  
//  //
//  //TMP36 Pin Variables
//  // boolean useTMP36 = true;  // set this to false if you don not use TMP36
//  boolean useTMP36 = false;
//  #define aref_voltage 3.3 // we tie 3.3V to ARef and measure it with a multimeter!
//  int tempPin = 1; //the analog pin the TMP36's Vout (sense) pin is connected to
//  //the resolution is 10 mV / degree centigrade with a
//  //500 mV offset to allow for negative temperatures
//  int tempReading; // the analog reading from the sensor
//  //
 

void draw(void) {
  //
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_profont15);
  //u8g.setFont(u8g_font_6x10); 
  //
  
  //********* display date at bottom of screen
  thisDay = String(myRTC.getDate(), DEC) + "/"; 
  thisMonth="";
  for (int i=0; i<=2; i++){
    thisMonth += monthString[monthIndex[myRTC.getMonth(Century)-1]+i];
  }  
  thisDay=thisDay + thisMonth + "/"; 
  thisDay=thisDay + String(myRTC.getYear() , DEC);
  const char* newDay = (const char*) thisDay.c_str(); 
  u8g.drawStr(32,63, newDay);   

  
 
  // *********display time in digital format
  thisTime="";
  thisTime=String(myRTC.getHour(h12, PM)) + ":";
  if (myRTC.getMinute() < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(myRTC.getMinute()) + ":";
  if (myRTC.getSecond() < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(myRTC.getSecond());
  const char* newTime = (const char*) thisTime.c_str();
  u8g.drawStr(2,10, newTime);  

  // for testing/debugging purposes only
  //Serial.println(newTime);
  
  
  // ***********Now draw the clock face
  u8g.drawCircle(clockCentreX, clockCentreY, 20); // main outer circle
  u8g.drawCircle(clockCentreX, clockCentreY, 2);  // small inner circle

  
  //hour ticks
  for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    angle=(angle/57.29577951) ; //Convert degrees to radians
    int x2=(clockCentreX+(sin(angle)*20));
    int y2=(clockCentreY-(cos(angle)*20));
    int x3=(clockCentreX+(sin(angle)*(20-2.5)));
    int y3=(clockCentreY-(cos(angle)*(20-2.5)));
    u8g.drawLine(x2,y2,x3,y3);
  }
  
  // display second hand
  float angle = myRTC.getSecond() *6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  int x3=(clockCentreX+(sin(angle)*(20)));
  int y3=(clockCentreY-(cos(angle)*(20)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  
  // display minute hand
  angle = myRTC.getMinute() * 6 ; // + int((myRTC.second() /60 ) * 6); 
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(clockCentreX+(sin(angle)*(20-3)));
  y3=(clockCentreY-(cos(angle)*(20-3)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  
  // display hour hand
  angle = myRTC.getHour(h12, PM) * 30 + int((myRTC.getMinute() / 12) * 6 )   ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(clockCentreX+(sin(angle)*(20-11)));
  y3=(clockCentreY-(cos(angle)*(20-11)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);

  
  //==========Temperature 
  // // now add temperature if needed
  // if (useTMP36==true) {
  // //getting the voltage reading from the temperature sensor
  //   tempReading = analogRead(tempPin);   
  // // converting that reading to voltage, for 3.3v arduino use 3.3
  //   float voltage = tempReading * aref_voltage;
  //   voltage /= 1024.0; 
  // // now print out the temperature
  //   int temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
  //   String thisTemp1 = String(temperatureC) + "C";
  //   // printing output as follows used less program storage space
  //   const char* thisTemp = (const char*) thisTemp1.c_str();
  //   u8g.drawStr(100,10,thisTemp); 
  //   // the print command could be used, but uses more memory
  //   //u8g.setPrintPos(100,10);
  //   //u8g.print(thisTemp1);
  // }



  //*********use DS3131 temp sensor OR Arduino internal temp sensor
   String thisTemp1;
   if (useRTCforTemp) {
    thisTemp1 = myRTC.getTemperature();
   } else {
    thisTemp1 = String(GetTemp());
   }

   thisTemp1 = thisTemp1 +  " C";
   // printing output as follows used less program storage space
   const char* thisTemp = (const char*) thisTemp1.c_str();
   u8g.drawStr(80,10,thisTemp);   
}

void setup(void) {
  //Serial.begin(9600);
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
  
  // rebuild the picture after some delay - we dont need this for real clock
  //delay(1000);
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
