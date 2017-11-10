/**
 * Nov 2017 
 * v1.1
 * Adapted to use current date/time from DS3231 RTC module
 * 
 * v1.2
 * Added option to display full date, DS3231 temp etc
 * 
 * ===========================
 * Nokia 5110 Random Clock
 * v. 1.0
 * Copyright (C) 2016 Robert Ulbricht
 * http://www.arduinoslovakia.eu
*/ 

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>

#define DS3231_ADDRESS 0x68     //same for DS3231 as well

#define DS3231_TEMPERATURE_MSB 0x11
#define DS3231_TEMPERATURE_LSB 0x12

char mon[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

byte zero = 0x00; //workaround for issue #527

//option to display digital clock, year etc

//option 1
// 22:34:54
// 8 Nov
//  2017
// 22.3 C
int opt=1;     //default

//option 2
// h=22
// m=54
// s=20
// d=8
// m=11
// y=17



// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);


void setup() {
  Wire.begin();
  // put your setup code here, to run once:
  display.begin();
  display.setContrast(45);    // 30 for non-backlit and 45 for backlit display 
  display.clearDisplay();
  display.display(); // show splashscreen

  //Serial.begin(9600);
  //drawClock(1,15,32);
  //drawClock(1,15,0);
}

void loop() {
//  int h=random(24);
//  int m=random(60);
//  int s=random(60);

 //Read data from DS3231 RTC module using Wire lib
 
 // Reset the register pointer
 Wire.beginTransmission(DS3231_ADDRESS);
 Wire.write(zero);
 Wire.endTransmission();

 Wire.requestFrom(DS3231_ADDRESS, 7);

 int s = bcdToDec(Wire.read());
 int m = bcdToDec(Wire.read());
 int h = bcdToDec(Wire.read() & 0b111111); //24 hour time
 int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
 int monthDay = bcdToDec(Wire.read());
 int month = bcdToDec(Wire.read());
 int year = bcdToDec(Wire.read());

  
  for(int i=0;i<10;i++)
    {
    drawClock(h,m,s,monthDay,month,year);
    delay(1000);
    s++;
    if(s>=60) m++;
    s=s%60;
    if(m>=60) h++;
    m=m%60;
    h=h%24;
    }
}


void drawClock(int h, int m, int s, int monthDay, int month, int year)
{
const int r=23;   //drawCircle is not drawing a proper circle...looks oval
const double rot=-M_PI/2;
double x,y,x0,y0,anglerad;
  
  display.clearDisplay();
  //display.drawPixel(0,0,BLACK);
  display.drawPixel(24,24,BLACK);
  display.drawCircle(24,24,r,BLACK);
  for(int i=0;i<12;i++)
    {
    int angled=360/12*i;
    anglerad=2*M_PI/12*i+rot;
    x=r*cos(anglerad);
    y=r*sin(anglerad);
    x0=(r-3)*cos(anglerad);
    y0=(r-3)*sin(anglerad);
    /*Serial.print(i);
    Serial.print(",");
    Serial.print(angled);
    Serial.print(",");
    Serial.print(anglerad);
    Serial.print(",");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.println("");*/
    display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);
    }


  anglerad=2*M_PI/12*(h%12)+2*M_PI/12/60*m+rot;
  x=(r-15)*cos(anglerad);
  y=(r-15)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);
  

  anglerad=2*M_PI/60*m+rot;
  x=(r-10)*cos(anglerad);
  y=(r-10)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);


  anglerad=2*M_PI/60*s+rot;
  x=(r-5)*cos(anglerad);
  y=(r-5)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);

//  // border
//  display.drawLine(0,0,3,0,BLACK);
//  display.drawLine(0,0,0,3,BLACK);
//  display.drawLine(0,47,3,47,BLACK);
//  display.drawLine(0,47,0,47-3,BLACK);
//  display.drawLine(83,0,83-3,0,BLACK);
//  display.drawLine(83,0,83,3,BLACK);
//  display.drawLine(83,47,83-3,47,BLACK);
//  display.drawLine(83,47,83,47-3,BLACK);

if(opt == 2) {
  // hour
  display.setCursor(50,0);
  display.print("h=");
  display.print(h);
  
  // minute
  display.setCursor(50,8);
  display.print("m=");
  display.print(m);
  // second
  display.setCursor(50,16);
  display.print("s=");
  display.print(s);
  
  // dayofmonth
  display.setCursor(50,24);
  display.print("d=");
  display.print(monthDay);
  // month
  display.setCursor(50,32);
  display.print("m=");
  display.print(month);
  // year
  display.setCursor(50,40);
  display.print("y=");
  display.print(year);
} else {
  //option 1
  // 22:34
  // 8 Nov
  // 2017
  // 22.3 C
  
  display.setCursor(50,0);
  display.print(h);
  s%2==0? display.print(":") : display.print(" ");  //blink every alternate sec

  //display 1 as 01, 2 as 02 etc. till 9 as 09 for min
  char dd[10][3]={"00","01","02","03","04","05","06","07","08","09"};
  m<10 ? display.print(dd[m]): display.print(m);

  
  display.setCursor(48,10);  
  display.print(monthDay);
  display.print(" ");
  display.print(mon[month-1]);
  display.setCursor(55,20); 
  display.print(2000+year);
  
  
}
  //print temp sensed by DS3231 RTC module
  
  display.setCursor(50,40);
  //display.print("T=");
  display.print(DS3231_getTemperature());
  
  display.display();  
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
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_ADDRESS, 2);

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

// supporting methods for DS3231
byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
 return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
 return ( (val/16*10) + (val%16) );
}
