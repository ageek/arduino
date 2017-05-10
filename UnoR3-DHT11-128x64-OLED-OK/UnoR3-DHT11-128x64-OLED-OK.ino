/*

  HelloWorld.pde
  
  "Hello World!" example code.
  
  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).
  
  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.
  
*/

#include <dht.h>

// Mesh facing front
// L 2 R
// 1 --> 5v
// 2 --> DigitalPin2
// 3 --> NC
// 4 --> GND


#define DHT11_PIN 2

dht DHT;

#include "U8glib.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device

// OLED connections L->R
// 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 


void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  //u8g.drawStr( 0, 10, "0-Hello World!");
  //u8g.drawStr( 0, 20, "1-Hello World!");
  
  // DHT11
  //http://www.avrfreaks.net/forum/how-do-i-print-variable-u8glib
  
  int chk = DHT.read11(DHT11_PIN);
  //display.print("Temp    =");
  //display.println(DHT.temperature);
  char tmp1_string[8];
  itoa(DHT.temperature, tmp1_string, 10);
  //u8g.drawStr( 10, 10, tmp1_string);
  u8g.print(DHT.temperature);

  //display.setTextColor(BLACK);
  //display.print("Humidity=");
  //display.println(DHT.humidity);
  char tmp2_string[8];
  itoa(DHT.humidity, tmp2_string, 10);
  //u8g.drawStr( 10, 40, tmp2_string);

  u8g.print(DHT.humidity);
}

void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }


  
  pinMode(8, OUTPUT);
}

void loop(void) {
//  for(;;)
//  {
//    u8g.firstPage();
//    do
//    {
//      draw();
//    } while ( u8g.nextPage() );
//    u8g_Delay(100);
//  }

  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}

