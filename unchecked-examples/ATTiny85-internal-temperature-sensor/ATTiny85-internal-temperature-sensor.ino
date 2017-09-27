#include "InternalTemperatureSensor.h"

InternalTemperatureSensor temperature( 1.0, TEMPERATURE_ADJUSTMENT ); // The 1.0 argument is ignored by default, see the c’tor comments

void setup() {

  temperature.init(); // Call init() in setup or every time after you modify ADCSRA or ADMUX

}


void loop() {

  // temperature.print(); // uncomment this to debug sensor output via serial connection

  int war_sensor_data = temperature.raw();             // This returns barely usable sensor output

  int temperature_in_celsius = temperature.in_c();   // This returns more usable temperature in degrees Celsius

}



