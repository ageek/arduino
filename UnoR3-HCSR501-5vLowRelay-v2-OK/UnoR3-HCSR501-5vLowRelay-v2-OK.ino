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

  while(millis()<13000)
  {
    digitalWrite(led,HIGH);
    delay(50);
    digitalWrite(led,LOW);
    delay(50);
  }
  digitalWrite(led,LOW);
  
}


void loop() 
{
  digitalWrite(in1,HIGH);
  digitalWrite(led,LOW);
  if(digitalRead(sensor)==HIGH)
  {
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
