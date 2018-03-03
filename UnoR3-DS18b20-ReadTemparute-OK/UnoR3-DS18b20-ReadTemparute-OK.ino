// https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806
// for ds18b20 clones from china, pin 1 and 3 are swapped
// See below alternative thats works irrspective of 1, 3 pin positions...center is always Pin2
// Wiring Tips: You can also wire this up with pins 1, and 3 to ground(both extremes), and the signal(any Arduino Digital pin)
//  connected to Pin2. Pin2 also needs to be pulled-up/connected to 5v via 4k7 pull up resistor.

// Just be careful, I bough a DS18B20 out of China and pins 1 & 3 are actually reversed from this tutorial. 
// that will give you a high reading. fell the sensor, it will be very hot to touch. Swap over pins 1 & 3 and you should be good to go
// for china clones: Pin1: Vdd, Pin3: GND, Pin2:signal
// with flat surface facing up, Left to right is : Pin1, Pin2 and Pin3

/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
void setup(void) 
{ 
 // start serial port 
 Serial.begin(9600); 
 Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Start up the library 
 sensors.begin(); 
} 
void loop(void) 
{ 
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.println("DONE"); 
/********************************************************************/
 Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   delay(1000); 
}
