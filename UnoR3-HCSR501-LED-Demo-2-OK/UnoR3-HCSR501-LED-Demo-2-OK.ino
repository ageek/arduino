    /////////////////////////////////////////////////////////////////
   //             Arduino PIR sensor Tutorial           v1.00     //
  //       Get the latest version of the code here:              //
 //      http://educ8s.tv/arduino-pir-sensor-tutorial           //
/////////////////////////////////////////////////////////////////


int ledPin = 13;
int pirPin = 3;
int val = 0;

void setup() 
{
  pinMode (ledPin,OUTPUT);
  pinMode (pirPin, INPUT);
}
void loop () 
{
  val = digitalRead(pirPin);
  digitalWrite(ledPin,val);

if (val == 1)
  digitalWrite(ledPin,LOW);
}

