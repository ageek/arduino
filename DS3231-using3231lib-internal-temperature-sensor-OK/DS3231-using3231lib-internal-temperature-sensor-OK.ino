#include "DS3231.h"
 
DS3231 RTC;
int tempC;
int tempF;
 
void setup() {
  //RTC.begin();
    // Start the serial interface
  Serial.begin(9600);
}
 
void loop() {
  tempC = RTC.getTemperature();
  tempF = (tempC * 1.8) + 32.0; // Convert C to F
 
  Serial.print(tempC);
  Serial.print(" C, ");
  Serial.print(tempF);
  Serial.print(" F");
  
  Serial.println();
  
  delay(1000);
}

