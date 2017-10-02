#include <avr/io.h>
#include "InternalTemperatureSensor.h"
//#include "SoftwareSerial.h"



//You can adjust the rolling average parameters, but overall I do not recommend making TEMPERATURE_SAMPLES 
//less than 20 and greater than 50. If it’s less than 20 then deviation grows significantly. Making 
//TEMPERATURE_SAMPLES greater than 50 doesn’t improve precision but wastes memory and slows down calculations.
//Also EXTREMES_RATIO defines how much of extreme values will be chopped, do not make it less than 2 (that will
//eliminate all values). Making it more than TEMPERATURE_SAMPLES will effective prevent removing extremes from 
//the samples.

// refer this for pinout: http://2.bp.blogspot.com/-gSqbotFHKfk/UberFXuAOBI/AAAAAAAAAZo/4IUnODX2KuE/s1600/attiny85_pins.png
// PB3/pin3 --> physical pin 2  , using standard IC pin numbering of ATTiny85
// PB4/pin4 --> physical pin 3   

// define Rx, Tx pins
// Rx of USB TTL should connect to -->  Tx of ATtiny85 PB4 ------> P4 of digispark 
// Tx of USB TTL should connect to -->  Rx of ATtiny85 PB3 ------> P3 of digispark
// define pins Rx=PB3, Tx=PB4 of digispark
//SoftwareSerial TinySerial(3, 4);   // TinySerial(Rx,Tx)



void InternalTemperatureSensor::init() {
  //analogReference( INTERNAL1V1 );
  // ATTiny85 datasheet p140 (17.13.2), p137 (17.12)
//// // Part A: my code   **************************Not working, something is wrong? 
// // shows this output:
// ######### 
// R:1021
// L:1023
// K:1010
// C:737
// F:1358
// #########
//  // Configure ADMUX
//  // Reset ADMUX to 0
//  ADMUX = 0;
//  // Set ADC4 to 1111 to sense temperature
//  // Selecting the ADC4 channel by writing the MUX[3:0] bits in ADMUX register to “1111” enables the temperature
//  // sensor
//  ADMUX |=  (1 << MUX3) |  (1 << MUX2) | (1 << MUX1) | (1 << MUX0) ;
//  
//  // Lets use 1.1v internal voltage reference
//  //REFS2, REFS1, REFS0
//  // 0      1       0     //Internal 1.1V Voltage Reference.
//  ADMUX |= (0 << REFS2); 
//  ADMUX |= (1 << REFS1); 
//  ADMUX |= (0 << REFS0);
//  
//  // disable auto trigger and disable interrupt
//  ADMUX &= (0 << ADATE);
//  ADMUX &= (0 << ADIE);
//  
//  // Enable ADC 
//  ADCSRA |= (1 << ADEN) | ( 1<< ADSC);
//  
//  // Set Prescaler to 64, 8MHz /64 = 125000 = 125KHz (50 < X < 200 KHz)
//  // 64 => 1  1  0
//  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS1);
  
  // Start conversion
  //ADCSRA |= (1 << ADSC); 
  //Wait for conversion to finish 
  //while (bit_is_set(ADCSRA, ADSC));  // taken care in method raw()
  
  
// // Part B: old code from web  **************************Tested working OK=======



  // Start with a clean slate
  ADMUX = 0;

  // Selecting the ADC4 channel by writing the MUX[3:0] bits in ADMUX register to “1111” enables the temperature sensor
  ADMUX |=  (1 << MUX3) |  (1 << MUX2) | (1 << MUX1) | (1 << MUX0) ;
  //ADMUX =  _BV(1111);                        // Select temperature sensor, //not working ...use the above line instead

  ADMUX &= ~ _BV( ADLAR );                     // Right-adjust result
  ADMUX |=  _BV( REFS1 );                      // Set Ref voltage
  ADMUX &= ~(  _BV( REFS0 ) |  _BV( REFS2 ) ); // to 1.1V

  // Configure ADCSRA
  ADCSRA &= ~( _BV( ADATE ) | _BV( ADIE ) );   // Disable autotrigger, Disable Interrupt
  ADCSRA |=  _BV(ADEN);                        // Enable ADC
  ADCSRA |=  _BV(ADSC);                        // Start first conversion
  
  // Seed samples
  int raw_temp;
  while( ( ( raw_temp = raw() ) < 0 ) );
  for( int i = 0; i < TEMPERATURE_SAMPLES; i++ ) {
    readings[i] = raw_temp;
  }
}


int InternalTemperatureSensor::in_lsb() {
  int readings_dup[TEMPERATURE_SAMPLES];
  int raw_temp;
  // remember the sample
  if( ( raw_temp = raw() ) > 0 ) {
    readings[pos] = raw_temp;
    pos++;
    pos %= TEMPERATURE_SAMPLES;
  }
  // copy the samples
  for( int i = 0; i < TEMPERATURE_SAMPLES; i++ ) {
    readings_dup[i] = readings[i];
  }
  // bubble extremes to the ends of the array
  int extremes_count = TEMPERATURE_SAMPLES / EXTREMES_RATIO;
  int swap;
  for( int i = 0; i < extremes_count; ++i ) { // percent of iterations of bubble sort on small N works faster than Q-sort
  for( int j = 0; j < TEMPERATURE_SAMPLES-1; j++ ) {
      if( readings_dup[i] > readings_dup[i+1] ) { // could be done with 3 XORs and no swap if you like fancy
        swap = readings_dup[i];
        readings_dup[i] = readings_dup[i+1];
        readings_dup[i+1] = swap;
      }
    }
  }
  // average the middle of the array
  int sum_temp = 0;
  for( int i = extremes_count; i < TEMPERATURE_SAMPLES - extremes_count; i++ ) {
    sum_temp += readings_dup[i];
  }
  return sum_temp / ( TEMPERATURE_SAMPLES - extremes_count * 2 );
}

int InternalTemperatureSensor::in_c() {
  return in_k() - 273;
}
int InternalTemperatureSensor::in_f() {
  return in_c() * 9 / 5 + 32;
}
int InternalTemperatureSensor::in_k() {
  return in_lsb() + offset; // for simplicty I'm using k=1, use the next line if you want K!=1.0
  //return (int)( in_lsb() * coefficient ) + offset;
}
int InternalTemperatureSensor::raw() {
  if( ADCSRA & _BV( ADSC ) ) {
    return -1;
  } else {
    int ret = ADCL | ( ADCH << 8 );   // Get the previous conversion result
    ADCSRA |= _BV(ADSC);              // Start new conversion
    return ret;
  }

}

