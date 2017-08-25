//http://www.arduinoecia.com.br/2014/11/arduino-modulo-rtc-ds1302.html?m=1
// my1302 module seems faulty, time gets reset   .may be a flat/faulty battery

// Program: Date and time with RTC module DS1302 
// Changes and adaptations: Arduino e Cia 
// 
// Based on the original Krodal program and the library virtuabotixRTC

// Load the virtuabotixRTC library 
#include "virtuabotixRTC.h"           

// Determines the pins attached to the 
// myRTC (clock, data, rst) 

virtuabotixRTC myRTC(2, 3, 4);

void setup()  
{      
  Serial.begin(9600);
 // initial date and time information 
  // After the information, comment the line below 
  // (seconds, minutes, hour, day of the week, day of the month, month, year) 
  myRTC.setDS1302Time(00, 11, 17, 6, 25, 8, 2017);
}

void loop()  
{
  // Read the IC information
  myRTC.updateTime(); 

  // Prints the information on the serial monitor 
  Serial.print("Data : ");
  // Calls the routine that prints the day of the week
  day_of_week(myRTC.dayofweek);
  Serial.print(", ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print("Time : ");
  // Add a 0 if the hour value is <10
  if (myRTC.hours < 10)
  {
    Serial.print("0");
  }
  Serial.print(myRTC.hours);
  Serial.print(":");
  // Add a 0 if the minutes value is <10
  if (myRTC.minutes < 10)
  {
    Serial.print("0");
  }
  Serial.print(myRTC.minutes);
  Serial.print(":");
  // Add a 0 if the sec value is <10
  if (myRTC.seconds < 10)
  {
    Serial.print("0");
  }
  Serial.println(myRTC.seconds);

  delay( 1000);
}

void day_of_week(int dia)
{
  switch (dia)
  {
    case 1:
    Serial.print("Sunday");
    break;
    case 2:
    Serial.print("Monday");
    break;
    case 3:
    Serial.print("Tuesday");
    break;
    case 4:
    Serial.print("Wednesday");
    break;
    case 5:
    Serial.print("Thursday");
    break;
    case 6:
    Serial.print("Friday");
    break;
    case 7:
    Serial.print("Saturday");
    break;
  }
}
