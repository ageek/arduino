/**
 * Nov 2017 
 * v1.1
 * Adapted to use current date/time from DS3231 RTC module
 * 
 * Nokia 5110 Random Clock
 * v. 1.0
 * Copyright (C) 2016 Robert Ulbricht
 * http://www.arduinoslovakia.eu
*/ 

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>

#define DS1307_ADDRESS 0x68     //same for DS3231 as well

byte zero = 0x00; //workaround for issue #527

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
  display.setContrast(45);
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
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero);
 Wire.endTransmission();

 Wire.requestFrom(DS1307_ADDRESS, 7);

 int s = bcdToDec(Wire.read());
 int m = bcdToDec(Wire.read());
 int h = bcdToDec(Wire.read() & 0b111111); //24 hour time
 int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
 int monthDay = bcdToDec(Wire.read());
 int month = bcdToDec(Wire.read());
 int year = bcdToDec(Wire.read());

  
  for(int i=0;i<10;i++)
    {
    drawClock(h,m,s);
    delay(1000);
    s++;
    if(s>=60) m++;
    s=s%60;
    if(m>=60) h++;
    m=m%60;
    h=h%24;
    }
}

void drawClock(int h, int m, int s)
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

  // hour
  display.setCursor(50,0);
  display.print("h=");
  display.print(h);
  anglerad=2*M_PI/12*(h%12)+2*M_PI/12/60*m+rot;
  x=(r-15)*cos(anglerad);
  y=(r-15)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);
  
  // minute
  display.setCursor(50,9);
  display.print("m=");
  display.print(m);
  anglerad=2*M_PI/60*m+rot;
  x=(r-10)*cos(anglerad);
  y=(r-10)*sin(anglerad);
  x0=0;
  y0=0;
  display.drawLine(24+x0,24+y0,24+x,24+y,BLACK);

  // second
  display.setCursor(50,18);
  display.print("s=");
  display.print(s);
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
  
  display.display();  
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
