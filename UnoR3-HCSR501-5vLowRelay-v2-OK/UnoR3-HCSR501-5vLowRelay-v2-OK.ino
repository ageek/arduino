//adapted from web
//Note: disable all serial outputs for this to work from any power source
int in1 = 8;
int sensor = 2;
int led = 13;
unsigned long t=0;

void setup() 
{
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(led, OUTPUT);
  
  digitalWrite(in1,HIGH);
  digitalWrite(led,LOW);
  Serial.println("Sensor Warming up, pls wait for 30sec ...");
  // Warm up time for PIR sensor usually 30 to 60 secs is recommended
  // lets keep it 30s
  while(millis()<30000)
  {
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(50);
  }
  digitalWrite(led,LOW);
  Serial.println("PIR Sensor is now ready !");
}


void loop() 
{
  digitalWrite(in1,HIGH);
  digitalWrite(led,LOW);
  if(digitalRead(sensor)==HIGH)
  {
    Serial.print("Motion detected at "); 
    Serial.print(millis()/1000 );
    Serial.println("th sec");
   t=millis();
   while(millis()<(t+5000))
   {
   digitalWrite(in1,LOW);
   digitalWrite(led,HIGH);
     if((millis()>(t+2300))&&(digitalRead(sensor)==HIGH))
      {
       t=millis();
      }
   }
  }
}
