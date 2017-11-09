// http://henrysbench.capnfatz.com/henrys-bench/arduino-displays/arduino-nokia-5110-with-u8glib-tutorial/
// Henry's Bench
// Arduino Nokia 5110 U8Glib Tutorial


#include "U8glib.h"

 

#define backlight 5
 
//Delcare the display and assign the pins

// AdaFruit lib to U8glib pin mappings
// pin 7 - Serial clock out (SCLK)3
// pin 6 - Serial data out (DIN)4
// pin 5 - Data/Command select (D/C)5
// pin 4 - LCD chip select (CS)7
// pin 3 - LCD reset (RST)6
//Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

//U8GLIB_PCD8544 u8g(8, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6

U8GLIB_PCD8544 u8g(3,4,7,5,6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6
 

 
void draw() {
 
  u8g.setFont(u8g_font_profont11);  // select font
  u8g.drawStr(0, 15, "Nokia 5110..");  // put string of display at position X, Y
  u8g.drawStr(29, 35, "HELLO!!!!");
  
  
}
 
void setup() {
  // Set Backlight Intensity
  // 0 is Fully bright
  // 255 is off
  analogWrite(backlight, 80);
  //
  // Enter the U8Glib Picture Loop
  //
  
}
 
void loop() { 
 u8g.firstPage(); 
  do {
    draw();
  } while( u8g.nextPage() ); 
  
}


