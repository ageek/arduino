#include <avr/io.h>
//#include <NewSoftSerial.h>
//NewSoftSerial lcd(255, 2);

void setup() {
 //lcd.begin(9600);
 //lcd.print("?f?B80");    // Clear display, set backlight
  Serial.begin(9600);
  Serial.println("ProminiTempSensor test!");
 ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);   // Set internal 1.1V reference, temperature reading
 delay(100);
 chipTempRaw(); // discard first sample
}

void loop() {
 float rawTemp;

 rawTemp = chipTempRaw(); // use next sample as initial average
 for (int i=2; i<2000; i++) {
   rawTemp += (chipTempRaw() - rawTemp) / float(i); // calculate running average
 }

 //Serial.print("Raw: ");       
 //Serial.println(rawTemp);
 Serial.print("Temp: "); 
 Serial.print(chipTemp(rawTemp)); 
 Serial.println(" C  ");
 delay(2000);
}

float chipTemp(float raw) {
 const float chipTempOffset = 336.59;
 const float chipTempCoeff = 1.17;
 return((raw - chipTempOffset) / chipTempCoeff);
}

int chipTempRaw(void) {
//  while((ADCSRA & _BV(ADSC)));                   // Wait for any ongoing conversion to complete
 ADCSRA &= ~(_BV(ADATE) |_BV(ADIE));            // Clear auto trigger and interrupt enable
 ADCSRA |= _BV(ADEN) | _BV(ADSC);               // Enable AD and start conversion
 while((ADCSRA & _BV(ADSC)));                   // Wait until conversion is finished
 return(ADCL | (ADCH << 8));
}
