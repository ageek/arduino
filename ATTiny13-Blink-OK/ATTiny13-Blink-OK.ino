// Attiny13 hello world blink program
// for burning bootloader follow this link:
//https://create.arduino.cc/projecthub/taunoerik/programming-attiny13-with-arduino-uno-07beba

// as per my custom h/w setup for flashing attiny85/attiny13 etc
// below code is a copy of my attiny85 blinky program

int ATTINY13_LED_PIN = 0;
void setup() {
 // initialize pin 4 (ATtiny leg 3) as an output.
 pinMode(ATTINY13_LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ATTINY13_LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(ATTINY13_LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(50);                       // wait for a second

  delay(1000);

  digitalWrite(ATTINY13_LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(ATTINY13_LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(50);                       // wait for a second


//
  digitalWrite(ATTINY13_LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(ATTINY13_LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(50);                       // wait for a second
  
  //delay(1500);             // wait for a second
} 
