// Ref: http://www.instructables.com/id/ATtiny-Watch-Core/

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


#include <avr/sleep.h>
#include <TinyWireM.h>
#include "ssd1306.h"
#include "WDT_Time.h"

// Pin connections
// SSD1306 OLED       ATTiny85/Digispark
// SCL ----------------- SCL / PB2 / P2
// SDA ----------------- SDA / PB0 / P0

// Vcc 
#define TIMEOUT 5000 // 5 seconds
static uint32_t display_timeout;

#define UNUSEDPIN 1
#define SETBUTTON 3
#define UPBUTTON  4

//#define SCREEN128X64 

SSD1306 oled;
bool sleeping = false;

void setup() {
  pinMode(UNUSEDPIN, INPUT_PULLUP);
  pinMode(SETBUTTON, INPUT_PULLUP);
  pinMode(UPBUTTON, INPUT_PULLUP);

  wdt_setup();

  TinyWireM.begin();
  oled.begin();
  oled.fill(0x00); // clear in black
  //set_display_timeout();

  // test
  setTime();
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

void enter_sleep() {
  oled.fill(0x00); // clear screen to avoid show old time when wake up
  oled.off();
  delay(2); // wait oled stable

  sleeping = true;
}

void wake_up() {
  sleeping = false;

  delay(2); // wait oled stable
  oled.on();

  set_display_timeout();
}

void set_display_timeout() {
  display_timeout = millis() + TIMEOUT;
}

/*
 * UI related
 */

// 0: time; 1: debug
static uint8_t display_mode = 0;
static uint8_t last_display_mode = 0;
// 0: none; 1: year; 2: month; 3: day; 4: hour; 5: minute
static uint8_t selected_field = 0;

void draw_oled() {

    // 1st rows: print date
    /* print_digits(col, page, size, factor, digits) */
    oled.print_digits(0, 0, 1, 1000, year(), false);
    /* draw_pattern(col, page, width, pattern) */
    oled.draw_pattern(29, 0, 2, 0b00010000); // dash
    oled.print_digits(32, 0, 1, 10, month(),  false);
    oled.draw_pattern(47, 0, 2, 0b00010000); // dash
    oled.print_digits(50, 0, 1, 10, day(),  false);
    // 2nd-4th rows: print time
    oled.print_digits(0, 1, 2, 10, hour(),  false);
    oled.draw_pattern(33, 2, 2, (second() & 1) ? 0b11000011 : 0); // blink colon
    oled.print_digits(35, 1, 2, 10, minute(),  false);

    oled.print_digits(72, 2, 1, 10, second(),  false);
    
    
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
  setTime(18, 10, second(), 11, 10, 2017);
  set_display_timeout(); // extent display timeout while user input
  //sleep_enable();
  
}

