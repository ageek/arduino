#ifndef _INTERNAL_TEMPERATURE_SENSOR_H_
#define _INTERNAL_TEMPERATURE_SENSOR_H_
#define TEMPERATURE_SAMPLES 40
#define TEMPERATURE_ADJUSTMENT -13
#define EXTREMES_RATIO 5
#define MAXINT 32767
#define MININT -32767


/*
 * Based on samples:
 * – The internal temp sensor has horrible deviation. It’s output varies alot (+/-10 degrees/sec), so requires
 *   some “smoothing”. I’m removing extreme values and running rolling avg on the rest. This returns ok
 *   results on 20 samples and more. You can reduce number of samples to reduce response time, but this
 *   will diminish precision.
 * – The linearity is ok and approximately 1.0 as the datasheet promised
 * – The offset need to be calculated for each chip separately and provided in c’tor
 *
 * In order to calaculate the offset hook up terminal to pin 2 (Serial TX) 9600 8N1 and use
 * temperature.print() method call. This will print out temperature readouts. Match it against your
 * calibrated thermometer attached to the chip. Adjust Tos in c’tor if necessary.
 * See chapter 17.12 of the ATtiny85 datasheet for a bit more details
 */
 
class InternalTemperatureSensor {
  int offset;
  float coefficient;
  int readings[TEMPERATURE_SAMPLES];
  int pos;

  public:
  InternalTemperatureSensor( float k, int o ) : offset( o ), coefficient( k ), pos( 0 ) {}
  // Call it every time you need to prepare the chip to read sensor (i.e. in setup)
  // If you’re using other ADCs besides temperature call it before each temperature reading
  void init();
  // Returns the current averaged temperature in LSB
  int in_lsb();
  // Returns the current averaged temperature in degrees Celsius
  int in_c();
  // Returns the current averaged temperature in degrees Fahrenheit
  int in_f();
  // Returns the current averaged temperature in Kelvins
  int in_k();
  // Returns the current raw temperature reading from sensor
  int raw();

};
#endif // _INTERNAL_TEMPERATURE_SENSOR_H_
