#include <dht.h>

// Mesh facing front
// L 2 R
// 1 --> 5v
// 2 --> DigitalPin2
// 3 --> NC
// 4 --> GND

dht DHT;

#define DHT11_PIN 2

void setup(){
  Serial.begin(9600);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(2000);
}

