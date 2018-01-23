// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
//http://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-ds3231-real-time-clock-modules-with-arduino/
// DS3231 and DS1307 use same libraries, register address etc...
//they are pin compatible

#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(1000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
    
    // This line sets the RTC with an explicit date & time, for example to set
    
    // 21st Jan 2018, noon 15:21:20
    //rtc.adjust(DateTime(2018, 1, 21, 15, 21, 20));
  }
  // if you want to set the time anyway, this does it everytime you upload you program
  // run it once and comment afterwards
  //rtc.adjust(DateTime(2018, 1, 21, 15, 22, 20));
}

void loop () {
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
//    Serial.print(" since midnight 1/1/1970 = ");
//    Serial.print(now.unixtime());
//    Serial.print("s = ");
//    Serial.print(now.unixtime() / 86400L);
//    Serial.println("d");
//    
//    // calculate a date which is 7 days and 30 seconds into the future
//    DateTime future (now + TimeSpan(7,12,30,6));
//    
//    Serial.print(" now + 7d + 30s: ");
//    Serial.print(future.year(), DEC);
//    Serial.print('/');
//    Serial.print(future.month(), DEC);
//    Serial.print('/');
//    Serial.print(future.day(), DEC);
//    Serial.print(' ');
//    Serial.print(future.hour(), DEC);
//    Serial.print(':');
//    Serial.print(future.minute(), DEC);
//    Serial.print(':');
//    Serial.print(future.second(), DEC);
//    Serial.println();
    
//    Serial.println();

    delay(1000);
}
