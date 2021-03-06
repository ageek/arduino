// https://forum.arduino.cc/index.php?topic=262986.15
// Works with: Digispark (Yes), Attiny85(Yes)
// Wire and  USI_TWO are copied from digistump lib folder e.g. C:\PoratableApps\arduino-1.8.1-windows\arduino-1.8.1\portable\packages\digistump\hardware\avr\1.6.7\libraries\Wire
// as the default ones bundled for ATtiny85 core are giving tons of compilation errors regarding TWI etc...
#include "Wire.h"
#include "SoftwareSerial.h"

// define Rx, Tx pins
// Rx of USB TTL should connect to -->  Tx of ATtiny85 PB4 ------> P4 of digispark 
// Tx of USB TTL should connect to -->  Rx of ATtiny85 PB3 ------> P3 of digispark
// define pins Rx=PB3, Tx=PB4 of digispark
SoftwareSerial TinySerial(3, 4);   // TinySerial(Rx,Tx)
#define Serial TinySerial   // little hack 


#define DS3231_I2C_ADDRESS 0x68
#define DS3231_TEMPERATURE_MSB 0x11
#define DS3231_TEMPERATURE_LSB 0x12

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

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(30, 23, 22, 7, 30, 9, 17);   //uncomment to set time for 1st  time
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

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

byte DS3231_get_MSB()
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(DS3231_TEMPERATURE_MSB);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 1);
  return Wire.read();
}

byte DS3231_get_LSB()
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(DS3231_TEMPERATURE_LSB);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 1);
  return Wire.read() >> 6;
}

void displayTemp()
{
  byte temp_msb = DS3231_get_MSB();
  byte temp_lsb = DS3231_get_LSB();

  Serial.print(temp_msb);

  switch(temp_lsb){
  case 0:
    Serial.println(".00");
    break;
  case 1 :
    Serial.println(".25");
    break;
  case 2:
    Serial.println(".50");
    break;
  case 3:
    Serial.println(".75");
    break;
  }
}

void loop()
{
  displayTime(); // display the real-time clock data on the Serial Monitor,
  // temp is not working properly, showing some junk values...TODO
  //displayTemp();  
  delay(1000); // every second
}

