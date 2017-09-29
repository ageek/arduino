// http://andrey.mikhalchuk.com/2011/06/20/reading-attiny854525-internal-temperature-sensor.html
#include "InternalTemperatureSensor.h"
#include "SoftwareSerial.h"

SoftwareSerial TinySerial(3, 4);   // TinySerial(Rx,Tx)
#define Serial TinySerial   // little hack 

// The 1.0 argument is ignored by default, see the c’tor comments
InternalTemperatureSensor temperature( 1.0, TEMPERATURE_ADJUSTMENT ); 

void setup() {
  //initialize serial
  TinySerial.begin(9600);
  temperature.init(); // Call init() in setup or every time after you modify ADCSRA or ADMUX
}


void loop() {

  // temperature.print(); // uncomment this to debug sensor output via serial connection
  int war_sensor_data = temperature.raw();             // This returns barely usable sensor output
  int temperature_in_celsius = temperature.in_c();   // This returns more usable temperature in degrees Celsius
  // print to serial
  //temperature.print();

  Serial.print( " R:" );
  Serial.println( temperature.raw(), DEC );
  Serial.print( " L:" );
  Serial.println( temperature.in_lsb(), DEC );
  Serial.print( " K:" );
  Serial.println( temperature.in_k(), DEC );
  Serial.print( " C:" );
  Serial.println( temperature.in_c(), DEC );
  Serial.print( " F:" );
  Serial.println( temperature.in_f(), DEC );
  Serial.println( " ######### " );  
  
  delay(4000);  //delay of 3s
  
}

// from other code samples  - not used,  may be removed to reduce sketch size
double GetTemp() 
{
   // Lets use ATTiny85 internal temperature sensor
   
  // https://playground.arduino.cc/Main/InternalTemperatureSensor
  //double GetTemp(void)

  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celsius.
  return (t);
}
