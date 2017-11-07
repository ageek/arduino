// http://gadgetzoneblog.blogspot.in/2013/11/a-simple-clock-using-nokia-5110-lcd.html

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Wire.h"
 
#define DS1307_ADDRESS 0x68     //same for DS3231 as well

byte zero = 0x00; //workaround for issue #527

// pin 7 - Serial clock out (SCLK)3
// pin 6 - Serial data out (DIN)4
// pin 5 - Data/Command select (D/C)5
// pin 4 - LCD chip select (CS)7
// pin 3 - LCD reset (RST)6
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

float i;
float ii;
float yy;
int xk;
int second1;
int minute1;
int hour1; //24 hour time
int weekDay1; //0-6 -> sunday - Saturday
int monthDay1;
int month1;
int year1;


void setup()   {
 Wire.begin();
 Serial.begin(9600);
 pinMode(10, OUTPUT);
 analogWrite(10, 950);
 display.begin();
 display.clearDisplay();
 display.setContrast(45);
 // to be used only once while setting time on DS3231 module
 //setDateTime();
}

void loop() {
//  analogWrite(10, ii);
// ii++ ;
 printDate();
i = i + 0.05; 

// this space can be reclaimed for displaying DOW, Temp etc...
if(xk==1)
{
yy = (sin(i) * 5) + 37;
   display.drawPixel(i*2.95, yy, WHITE);
yy = (sin(i*-1) * 5) + 37;
   display.drawPixel(i*2.95, yy, WHITE);
}
else
{
yy = (sin(i) * 5) + 37;
   display.drawPixel(i*2.95, yy, BLACK);
yy = (sin(i*-1) * 5) + 37;
   display.drawPixel(i*2.95, yy, BLACK);  
} 
 display.display();
 delay(1);    //change as needed
 
if(i>33){
 i=0;
 //display.clearDisplay();
  second1=0;
  minute1=0;
  hour1=0; 
  weekDay1=0; 
  monthDay1=0;
  month1=0;
  year1=0;
  if(xk==1)
  {
    xk = 0;
  }
  else
  {
   xk=1;
  }  
} 

// if(ii>1023){
//  ii=0;

// } 
}
void setDateTime(){

 byte second =      45; //0-59
 byte minute =      04; //0-59
 byte hour =        20; //0-23
 byte weekDay =     1; //1-7
 byte monthDay =    25; //1-31
 byte month =       11; //1-12
 byte year  =       13; //0-99

 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero); //stop Oscillator

 Wire.write(decToBcd(second));
 Wire.write(decToBcd(minute));
 Wire.write(decToBcd(hour));
 Wire.write(decToBcd(weekDay));
 Wire.write(decToBcd(monthDay));
 Wire.write(decToBcd(month));
 Wire.write(decToBcd(year));

 Wire.write(zero); //start 

 Wire.endTransmission();

}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
 return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
 return ( (val/16*10) + (val%16) );
}

void printDate(){

 // Reset the register pointer
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero);
 Wire.endTransmission();

 Wire.requestFrom(DS1307_ADDRESS, 7);

 int second = bcdToDec(Wire.read());
 int minute = bcdToDec(Wire.read());
 int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
 int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
 int monthDay = bcdToDec(Wire.read());
 int month = bcdToDec(Wire.read());
 int year = bcdToDec(Wire.read());
 
 display.setTextSize(1);


    if(month!=month1)
 {
 display.setCursor(10,0);  
 display.setTextColor(WHITE);
  display.print(month1);
 display.setCursor(10,0);  
   if((month<=10) and (month>0))
 {
   display.print("0");
 }     
 display.print(month1);
   display.setCursor(10,0);
 display.setTextColor(BLACK);
   if(month<10)
 {
   display.print("0");
 } 
 display.print(month); 
 
 month1 = month;
  }        

 
 

  display.setCursor(26,0);
 display.print("/");
 

    if(monthDay!=monthDay1)
 {
 display.setCursor(34,0);  
 display.setTextColor(WHITE);
  display.print(monthDay1);
 display.setCursor(34,0);  
   if((monthDay<=10) and (monthDay>0))
 {
   display.print("0");
 }     
 display.print(monthDay1);
   display.setCursor(34,0);
 display.setTextColor(BLACK);
   if(monthDay<10)
 {
   display.print("0");
 }       
 display.print(monthDay); 
 monthDay1 = monthDay;
  }        

  display.setCursor(50,0);
 display.print("/");
 

   if(year!=year1)
 {
 display.setCursor(58,0);  
 display.setTextColor(WHITE);
  display.print(year1);
 display.setCursor(58,0);  
   if((year<=10) and (year>0))
 {
   display.print("0");
 }   
 display.print(year1);
   display.setCursor(58,0);
 display.setTextColor(BLACK);
    if(year<10)
 {
   display.print("0");
 }     
 display.print(year); 
 year1 = year;
  }       

display.setTextSize(2);
   if(hour!=hour1)
 {
 display.setCursor(0,12);  
 display.setTextColor(WHITE);
   display.print(hour1);
 display.setCursor(0,12);  
   if((hour<=10) and (hour>0))
 {
   display.print("0");
 }   
 display.print(hour1);
   display.setCursor(0,12);
 display.setTextColor(BLACK);
    if(hour<10)
 {
   display.print("0");
 }   
 display.print(hour); 
 hour1 = hour;
  }     

  display.setCursor(21,12);
 display.print(":");
 

   if(minute!=minute1)
 {
 display.setCursor(31,12);  
 display.setTextColor(WHITE);
   display.print(minute1);
 display.setCursor(31,12);  
   if((minute<=10) and (minute>0))
 {
   display.print("0");
 } 
 display.print(minute1);
   display.setCursor(31,12);
 display.setTextColor(BLACK);
     if(minute<10)
 {
   display.print("0");
 } 
 display.print(minute); 
 minute1 = minute;
  }   

  display.setCursor(52,12);
 display.print(":");

  
 if(second!=second1)
 {
 display.setCursor(60,12);  
 display.setTextColor(WHITE);
 display.print(second1);
 display.setCursor(60,12);  
   if((second<=10) and (second>0))
 {
   display.print("0");
 } 
 display.print(second1);
   display.setCursor(60,12);
 display.setTextColor(BLACK);
   if(second<10)
 {
   display.print("0");
 } 
 display.print(second); 
 second1 = second;
  }   


}
