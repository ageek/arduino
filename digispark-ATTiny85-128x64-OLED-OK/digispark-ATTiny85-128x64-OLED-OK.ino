// http://www.instructables.com/id/ATtiny-Watch-Core/
// Search for  "Solved! i use this code:"
#include "SSD1306_minimal.h"
#include <avr/pgmspace.h>

#define DEG "\xa7" "C"

SSD1306_Mini oled;

void setup(){ 
oled.init(0x3c);
oled.clear();
delay(1000);
} 


void loop(){
oled.startScreen();
oled.cursorTo(15,1);
oled.printString( "Hello Oled_Attiny85 !!!"); 
}
