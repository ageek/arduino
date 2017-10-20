#include <DigisparkOLED.h>
#include <Wire.h>
// ============================================================================
// libraries used: 
//  Using library DigisparkOLED in folder: C:\PoratableApps\arduino-1.8.1-windows\arduino-1.8.1\portable\packages\digistump\hardware\avr\1.6.7\libraries\DigisparkOLED (legacy)
//  Using library Wire in folder: C:\PoratableApps\arduino-1.8.1-windows\arduino-1.8.1\portable\packages\digistump\hardware\avr\1.6.7\libraries\Wire (legacy)

// memory usage: 
//  Sketch uses 5328 bytes (88%) of program storage space. Maximum is 6012 bytes.
//  Global variables use 111 bytes of dynamic memory

// Pin connections
//  SSD1306 OLED       ATTiny85/Digispark
//  SCL ----------------- SCL / PB2 / P2
//  SDA ----------------- SDA / PB0 / P0


#define DS3231_I2C_ADDRESS 0x68
#define DS3231_TEMPERATURE_MSB 0x11
#define DS3231_TEMPERATURE_LSB 0x12

//char dow[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char dow[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

char twoDig[10][3] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09"};


void setup() {
  // put your setup code here, to run once:
  oled.begin();

  // put your main code here, to run repeatedly:
  //oled.fill(0xFF); //fill screen with color
  oled.clear();
  delay(1000);
}

void loop() {
  
//  oled.clear(); //all black
//  delay(1000);
//  //usage: oled.setCursor(X IN PIXELS, Y IN ROWS OF 8 PIXELS STARTING WITH 0);
//  oled.setCursor(0, 0); //top left
//  oled.setFont(FONT8X16);
//  oled.println(F("DIGISTUMP")); //wrap strings in F() to save RAM!
//  oled.print(F("2017-10-17"));
//  oled.setFont(FONT6X8);
//  oled.println(F(" OLED!"));
//  oled.setCursor(0, 4); //two rows down because the 8x16 font takes two rows of 8
//  oled.println(F("test")); //println will move the cursor 8 or 16 pixels down (based on the front) and back to X=0
//  oled.print(F("test test test test test")); //lines auto wrap

  
  byte year, month, dayOfMonth, hour, minute, second, dayOfWeek;

  //read data from ds3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  // Print year
  oled.setCursor(0, 0); //top left
  oled.setFont(FONT8X16);  
  oled.print((int)(2000+year));
  oled.print(F("/"));
  oled.print((int)month);
  oled.print(F("/"));
  oled.print((int)dayOfMonth);
  oled.print(F(" "));
  oled.println(dow[(int)dayOfWeek-1]);
  

  // print time
  hour < 10 ? oled.print(twoDig[(int)hour]) : oled.print((int)hour);
  oled.print(F(":"));
  
  minute < 10 ? oled.print(twoDig[(int)minute]) : oled.print((int)minute);
  oled.print(F(":"));
  
  second < 10 ? oled.print(twoDig[(int)second]) : oled.print((int)second);

  // print temp   -- takes 111% of digisparks total 6K memory...need to do edits to fit in digi..will work in attiny85 though
  //float ds3231temp = DS3231_getTemperature();
  //oled.print(ds3231temp); 
  //oled.println(F(" C")); 
  //delay(1000); 
}


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





//void draw_oled() {
//
//    byte year, month, dayOfMonth, hour, minute, second, dayOfWeek;
//
//    //read data from ds3231
//    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
//
//    // 1st rows: print date
//    // void print_digits(uint8_t col, uint8_t page, uint8_t font_size, uint32_t factor, uint32_t digits, bool invert_color);
//    /* print_digits(col, page, size, factor, digits) */
//    int start=20;
//    oled.print_digits(start+0, 0, 1, 1000, 2000+year, false);
//    /* draw_pattern(col, page, width, pattern) */
//    oled.draw_pattern(start+29, 0, 2, 0b00010000); // dash
//    oled.print_digits(start+32, 0, 1, 10, month,  false);
//    oled.draw_pattern(start+47, 0, 2, 0b00010000); // dash
//    oled.print_digits(start+50, 0, 1, 10, dayOfMonth,  false);
//    
//    // print ds3231 temp in int...will add oled.print_flaot() later
//    int ds3231temp = (int) DS3231_getTemperature();
//    oled.print_digits(95, 0, 1, 10, ds3231temp,  false);    
//
//    // print day
//    //oled.print_string()
//    
//    // 2nd-4th rows: print time
//    oled.print_digits(20, 1, 2, 10, hour,  false);
//    // TODO: blink every second, not on every alternate second 
//    oled.draw_pattern(53, 2, 2, (second & 1) ? 0b11000011 : 0); // blink colon
//    oled.print_digits(57, 1, 2, 10, minute,  false);
//    oled.print_digits(95, 2, 1, 10, second,  false);
//       
//}




