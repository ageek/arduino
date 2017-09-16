// "ATtiny85+TM1637+DS3231+H20 lipo" wristwatch v0.1
//
#include <TinyWireM.h>
#include "TinyRTClib.h"
#include "TM1637.h"

TM1637 tm1637(3 /*CLK*/, 4/*DIO*/);

RTC_DS1307 RTC;   // SDA=0, SCL=2

uint8_t h,l;
long last;
DateTime now;

void setup () {
  RTC.begin();

  if (! RTC.isrunning()) {
    while (true) {}
  }

  TinyWireM.begin();

  tm1637.init();
  tm1637.set(2);    //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637.point(1);

  last=RTC.now().unixtime();
}

void loop () {      // duration(1xRTC.now() plus display on TM1637) is <4ms
  do {
    now = RTC.now();
  } while (now.unixtime() == last);

  last = now.unixtime();

  h = (last%7 < 2) ? now.hour()   : now.minute();
  l = (last%7 < 2) ? now.minute() : now.second();
  
  tm1637.display(0,h/10);
  tm1637.display(1,h%10);
  tm1637.display(2,l/10);
  tm1637.display(3,l%10);
}


