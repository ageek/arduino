// Ref: http://www.instructables.com/id/ATtiny-Watch-Core/
// Following is a trimmed down version of tiny85 watch core(lot many things removed) from above link....
// in case you want the original pls refer above link
// Supports reading date, time and temp from DS3231 chip


// Memory usage
//On digispark: 
//  Sketch uses 5110 bytes (84%) of program storage space. Maximum is 6012 bytes.
//  Global variables use 86 bytes of dynamic memory.

//on attiny85


// TODO: 
//  0. clean up all unneeded defines, methods etc to reduce memory usage
//  1. Remove WDT_time and include RTC DS3231 for real time date and time
//  2. Display temperature either using ATTiny's internal temp sensor or DS3231's temp sensor which is more accurate
//  3. Remove the random dots  printed across the OLED screen

// Rev 1: all sleep() etc disabled, not intended to run Yr long on CR2032 battery
// Rev 2: Pending


#include <TinyWireM.h>
#include "ssd1306.h"

#define Wire TinyWireM    // hack to avoid too many code changes

// Pin connections
// SSD1306 OLED       ATTiny85/Digispark
// SCL ----------------- SCL / PB2 / P2
// SDA ----------------- SDA / PB0 / P0

//// Vcc 
//#define TIMEOUT 5000 // 5 seconds
//static uint32_t display_timeout;


//#define SCREEN128X64 

SSD1306 oled;

#define DS3231_I2C_ADDRESS 0x68
#define DS3231_TEMPERATURE_MSB 0x11
#define DS3231_TEMPERATURE_LSB 0x12

char dow[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

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

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
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
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);

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



void setup() {
  TinyWireM.begin();
  oled.begin();
  oled.fill(0x00); // clear in black
  // set time using DS3231 chip
  // void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
  // Sunday=1 ... Saturday=7
  // run once while setting time for DS3231 chip
  //setDS3231time(40,  40,  22,  1,  15,  10,  17);    // working OK, 
  
  // following line sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {
  //disabled all kinds of sleep for initial testing purposes

//  if (sleeping) {
//    system_sleep();
//  } else {
//    if (millis() > display_timeout) {
//      // disable sleep for testing
//      enter_sleep();
//    } else {
//      draw_oled();
//    }
//  }

  draw_oled();
}

//void enter_sleep() {
//  oled.fill(0x00); // clear screen to avoid show old time when wake up
//  oled.off();
//  delay(2); // wait oled stable
//
//  sleeping = true;
//}
//
//void wake_up() {
//  sleeping = false;
//
//  delay(2); // wait oled stable
//  oled.on();
//
//  set_display_timeout();
//}
//
//void set_display_timeout() {
//  display_timeout = millis() + TIMEOUT;
//}

/*
 * UI related
 */

//// 0: time; 1: debug
//static uint8_t display_mode = 0;
//static uint8_t last_display_mode = 0;
//// 0: none; 1: year; 2: month; 3: day; 4: hour; 5: minute
//static uint8_t selected_field = 0;

void draw_oled() {

    // get the values from DS3231
    // use dummy for now
//    byte year = 2017;
//    byte month = 10;
//    byte dayOfMonth = 12;
//    byte hour = 21;
//    byte minute = 28;
//    byte second = 27;
//    byte dayOfWeek;

    byte year, month, dayOfMonth, hour, minute, second, dayOfWeek;

    //read data from ds3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

    // 1st rows: print date
    // void print_digits(uint8_t col, uint8_t page, uint8_t font_size, uint32_t factor, uint32_t digits, bool invert_color);
    /* print_digits(col, page, size, factor, digits) */
    int start=20;
    oled.print_digits(start+0, 0, 1, 1000, 2000+year, false);
    /* draw_pattern(col, page, width, pattern) */
    oled.draw_pattern(start+29, 0, 2, 0b00010000); // dash
    oled.print_digits(start+32, 0, 1, 10, month,  false);
    oled.draw_pattern(start+47, 0, 2, 0b00010000); // dash
    oled.print_digits(start+50, 0, 1, 10, dayOfMonth,  false);
    
    // print ds3231 temp in int...will add oled.print_flaot() later
    int ds3231temp = (int) DS3231_getTemperature();
    oled.print_digits(95, 0, 1, 10, ds3231temp,  false);    

    // print day
    //oled.print_string()
    
    // 2nd-4th rows: print time
    oled.print_digits(20, 1, 2, 10, hour,  false);
    // TODO: blink every second, not on every alternate second 
    oled.draw_pattern(53, 2, 2, (second & 1) ? 0b11000011 : 0); // blink colon
    oled.print_digits(57, 1, 2, 10, minute,  false);
    oled.print_digits(95, 2, 1, 10, second,  false);


    
    
}

// orig method - commented 
//void draw_oled() {
//  if (display_mode != last_display_mode) {
//    oled.fill(0x00);
//    last_display_mode = display_mode;
//  }
//  if (display_mode == 0) {
//    // 1st rows: print date
//    /* print_digits(col, page, size, factor, digits) */
//    oled.print_digits(0, 0, 1, 1000, year(), (selected_field == 1) ? true : false);
//    /* draw_pattern(col, page, width, pattern) */
//    oled.draw_pattern(29, 0, 2, 0b00010000); // dash
//    oled.print_digits(32, 0, 1, 10, month(), (selected_field == 2) ? true : false);
//    oled.draw_pattern(47, 0, 2, 0b00010000); // dash
//    oled.print_digits(50, 0, 1, 10, day(), (selected_field == 3) ? true : false);
//    // 2nd-4th rows: print time
//    oled.print_digits(0, 1, 3, 10, hour(), (selected_field == 4) ? true : false);
//    oled.draw_pattern(31, 2, 2, (second() & 1) ? 0b11000011 : 0); // blink colon
//    oled.print_digits(34, 1, 3, 10, minute(), (selected_field == 5) ? true : false);
//  } else if (display_mode == 1) {
//    oled.print_digits(0, 0, 1, 10000, wdt_get_interrupt_count(), false);
//    oled.print_digits(0, 1, 1, 10000, readVcc(), false);
//  }
//}

//// PIN CHANGE interrupt event function
//ISR(PCINT0_vect)
//{
//  sleep_disable();
//
//  if (sleeping) {
//    wake_up();
//  } else {
//    if (digitalRead(SETBUTTON) == LOW) { // SET button pressed
//      selected_field++;
//      if (selected_field > 5) selected_field = 0;
//      if (selected_field > 0) display_mode = 0;
//    } else if (digitalRead(UPBUTTON) == LOW) { // UP button pressed
//      if (selected_field == 0) {
//        display_mode++; // toggle mode;
//        if (display_mode > 1) display_mode = 0;
//      } else {
//        int set_year = year();
//        int set_month = month();
//        int set_day = day();
//        int set_hour = hour();
//        int set_minute = minute();
//  
//        if (selected_field == 1) {
//          set_year++; // add year
//          if (set_year > 2069) set_year = 1970; // loop back
//        } else if (selected_field == 2) {
//          set_month++; // add month
//          if (set_month > 12) set_month = 1; // loop back
//        } else if (selected_field == 3) {
//          set_day++; // add day
//          if (set_day > getMonthDays(CalendarYrToTm(set_year), set_month)) set_day = 1; // loop back
//        } else if (selected_field == 4) {
//          set_hour++; // add day
//          if (set_hour > 23) set_hour = 0; // loop back
//        } else if (selected_field == 5) {
//          set_minute++; // add day
//          if (set_minute > 59) set_minute = 0; // loop back
//        }
//        setTime(set_hour, set_minute, second(), set_day, set_month, set_year);
//      }
//    }
//  }
//
//  set_display_timeout(); // extent display timeout while user input
//  sleep_enable();
//}

// settime directly 
void setTime(void) {
  // default code on digispark taks 101% memory...so for now set time direclty as below
  //setTime(set_hour, set_minute, second(), set_day, set_month, set_year);
  // invoke second() from WDT_Time
  //setTime(18, 10, 22, 11, 10, 2017);  
}

