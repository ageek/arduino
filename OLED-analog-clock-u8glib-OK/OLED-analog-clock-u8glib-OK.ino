// displays a dynamic analog clock --- Shows the date,time etc set via the setcommand, my DS1302 seems faulty....
// TODO:fix  DS1302  update of analog time
// BOM: Arduino (pref Uno), DS1302RTC module,0.96inch SSD1306 OLED display
// NOTE: Below mentioned TMP36 ,RTC1307 are not being used in actual compiled code
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
RTC DS1307 
Optional Temperature Sensor TMP 36
Wire RTC:
  VCC +5v
  GND GND
  SDA Analog pin 4
  SCL Analog pin 5
Wire OLED:
  VCC +5v
  GND GND
  SDA Analog pin 4
  SCL Analog pin 5
Wire TMP36:
  VCC +3.3v
  GND GND
  Out Analog pin 1
  
Connect ARef to 3.3v on Arduino
******************************************************************************/

// Add libraries
  #include "U8glib.h"
  #include <SPI.h>
  #include <Wire.h>
  #include "RTClib.h"

// Load the virtuabotixRTC library 
  #include "virtuabotixRTC.h"    


// setup u8g object
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 

         
// Determines the pins attached to the digital pins of Arduino
// myRTC (clock, data, rst) 
virtuabotixRTC myRTC(2, 3, 4);



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
  // Read the IC information
  myRTC.updateTime();
  
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_profont15);
  //u8g.setFont(u8g_font_6x10); 
  //
  
  //********* display date at bottom of screen
  thisDay = String(myRTC.dayofmonth, DEC) + "/"; 
  thisMonth="";
  for (int i=0; i<=2; i++){
    thisMonth += monthString[monthIndex[myRTC.month-1]+i];
  }  
  thisDay=thisDay + thisMonth + "/"; 
  thisDay=thisDay + String(myRTC.year , DEC);
  const char* newDay = (const char*) thisDay.c_str(); 
  u8g.drawStr(32,63, newDay);   

  
 
  // *********display time in digital format
  thisTime="";
  thisTime=String(myRTC.hours) + ":";
  if (myRTC.minutes < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(myRTC.minutes) + ":";
  if (myRTC.seconds < 10){ thisTime=thisTime + "0";} // add leading zero if required
  thisTime=thisTime + String(myRTC.seconds);
  const char* newTime = (const char*) thisTime.c_str();
  u8g.drawStr(10,10, newTime);  

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
  float angle = myRTC.seconds *6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  int x3=(clockCentreX+(sin(angle)*(20)));
  int y3=(clockCentreY-(cos(angle)*(20)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  //
  // display minute hand
  angle = myRTC.minutes * 6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(clockCentreX+(sin(angle)*(20-3)));
  y3=(clockCentreY-(cos(angle)*(20-3)));
  u8g.drawLine(clockCentreX,clockCentreY,x3,y3);
  //
  // display hour hand
  angle = myRTC.hours * 30 + int((myRTC.minutes / 12) * 6 )   ;
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
//
//
}

void setup(void) {
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
  myRTC.setDS1302Time(18, 52, 19, 6, 25, 8, 2017);

}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(1000);
}

