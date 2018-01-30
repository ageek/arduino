// Program to demonstrate the MD_Parola library
//
// Display the time in one zone and other information scrolling through in
// another zone.
// - Time is shown in a user defined fixed width font
// - Scrolling text uses the default font
// - Temperature display uses user defined characters
// - Optional use of DS1307 module for time and DHT11 sensor for temp and humidity
// - DS1307 library (MD_DS1307) found at https://github.com/MajicDesigns/DS1307
// - DHT11 library (DHT11_lib) found at http://arduino.cc/playground/Main/DHT11Lib
//
// NOTE: MD_MAX72xx library must be installed and configured for the LED
// matrix type being used. Refer documentation included in the MD_MAX72xx
// library or see this link:
// https://majicdesigns.github.io/MD_MAX72XX/page_hardware.html
//

// Use the DHT11 temp and humidity sensor
#define USE_DHT11 0

// Use the DS1307/DS3231 clock module
// same code works for both the modules, as register address ranges remains sames
#define USE_DS1307 1

// Header file includes
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include "Font_Data.h"

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define MAX_DEVICES 4

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10


#if USE_DHT11
#include <dht11.h>

#define DHT11_PIN 2

dht11 DHT11;
#endif

#if USE_DS1307
#include "MD_DS1307.h"
#include <Wire.h>
#endif

// Hardware SPI connection
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define SPEED_TIME  50
#define PAUSE_TIME  20

#define MAX_MESG  20

// Turn on debug statements to the serial output
#define DEBUG 1

// Global variables
char szTime[9];    // mm:ss\0
char szMesg[MAX_MESG+1] = "";


uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 }; // Deg C
uint8_t degF[] = { 6, 3, 3, 124, 20, 20, 4 }; // Deg F

char *mon2str(uint8_t mon, char *psz, uint8_t len)

// Get a label from PROGMEM into a char array
{
  static const __FlashStringHelper* str[] =
  {
    F("Jan"), F("Feb"), F("Mar"), F("Apr"),
    F("May"), F("Jun"), F("Jul"), F("Aug"),
    F("Sep"), F("Oct"), F("Nov"), F("Dec")
  };

  strncpy_P(psz, (const char PROGMEM *)str[mon-1], len);
  psz[len] = '\0';

  return(psz);
}

char *dow2str(uint8_t code, char *psz, uint8_t len)
{
  static const __FlashStringHelper*	str[] =
  {
    F("Sunday"), F("Monday"), F("Tuesday"),
    F("Wednesday"), F("Thursday"), F("Friday"),
    F("Saturday")
  };

  strncpy_P(psz, (const char PROGMEM *)str[code-1], len);
  psz[len] = '\0';

  return(psz);
}

void getTime(char *psz, bool f = true)
// Code for reading clock time
{
  RTC.readTime();
  sprintf(psz, "%02d%c%02d", RTC.h, (f ? ':' : ' '), RTC.m);
//  sprintf(psz, "%02d%c%02d", RTC.h, ':', RTC.m);
}

void getDate(char *psz)
// Code for reading clock date
{
  char	szBuf[10];
  RTC.readTime();
  sprintf(psz, "%d %s %04d", RTC.dd, mon2str(RTC.mm, szBuf, sizeof(szBuf)-1), RTC.yyyy);
}

void setup(void)
{
//  // pass number of zones
//  P.begin();
//  P.setInvert(false);
//
//  //custom zone boundaries for 4 in 1 module kit
////  P.setZone(0, 0, 1);
////  P.setZone(1, 2, 3);
////  P.setZone(0, 0, MAX_DEVICES-5);
////  P.setZone(1, MAX_DEVICES-4, MAX_DEVICES-1);
//  P.setFont(numeric7Seg);
//
////  let zone 0 use default fonts
////  P.setFont(0, numeric7Seg);
//  
//
//  P.displayText(szTime, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
////  P.displayZoneText(0, szMesg, PA_LEFT, SPEED_TIME, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
//
  // set intensity from 0(lowest) to 15(max)
//  P.setIntensity(1);
//  P.addChar('$', degC);
  
//
  RTC.control(DS1307_CLOCK_HALT, DS1307_OFF);
  RTC.control(DS1307_12H, DS1307_OFF);


  P.begin();
  P.setFont(numeric7Seg);
  P.setIntensity(1);
  
  getTime(szTime, true);
  P.displayText(szTime, PA_CENTER, 100, 0, PA_WIPE_CURSOR, PA_PRINT);
  //P.displayText(szTime, PA_CENTER, P.getSpeed(), PAUSE_TIME, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!P.displayAnimate());
}

void loop(void)
{
  static uint32_t lastTime = 0; // millis() memory
  static bool flasher = false;  // seconds passing flasher
  

    P.setIntensity(1);
    P.addChar('$', degC);
    
//v 2.0
//  if(P.displayAnimate()) 
  { 
    //P.setTextEffect(PA_PRINT, PA_PRINT);

    RTC.readTime();
    getTime(szTime, flasher);
    P.displayText(szTime, PA_CENTER, 10, 0, PA_PRINT, PA_PRINT);
    while (!P.displayAnimate());
  
    if (RTC.s==5) {
     //read temp from ds3231 chip       
     dtostrf(RTC.DS3231_getTemperature(), 3, 1, szMesg);
     strcat(szMesg, "$");
     //strcpy(szTime, szMesg);
     P.displayText(szMesg, PA_CENTER, 100, 3000, PA_SCROLL_UP, PA_SCROLL_UP);
     //delay(2000);     //blank screen for x mili seconds

     while (!P.displayAnimate()) ; // animates and returns true when an animation is completed
        
//     P.displayClear();
//     P.displayReset();

     char msgBuf[21]="";
     getDate(msgBuf);
     //dow2str(RTC.dow, msgBuf, MAX_MESG);
     P.displayText(msgBuf, PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
     while (!P.displayAnimate()) ; // animates and returns true when an animation is completed
     
     //P.displayClear();
     //P.displayReset();
    }
        
//    switch (display)
//    {
//      case 0: // Temperature deg C
//        //P.setTextEffect(PA_PRINT, PA_PRINT);
//        //display++;
//
//        break;
//
//      case 1: // day of week
//        P.setTextEffect(0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
//        display++;
//        dow2str(RTC.dow, szMesg, MAX_MESG);
//        break;
//
//      default:  // Calendar
//        P.setTextEffect(0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
//        display = 0;
//        getDate(szMesg);
//        break;
//    }
//
//    P.displayReset();
  }

  // Finally, adjust the time string if we have to
  // changed diff to 500ms for flashing dots every sec 
  // default was 1000ms
  if (millis() - lastTime >= 500)
  {
    lastTime = millis();
    getTime(szTime, flasher);
    flasher = !flasher;
  }     
}
