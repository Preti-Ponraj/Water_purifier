#define flow_sensor 2
volatile int flow_freq;
float vol=0.0,l_minute;
unsigned long currenttime,looptime;

void flow()
{
  flow_freq++;
}

void setup()
{
  Serial.begin(9600);
  pinMode(flow_sensor,INPUT);
  digitalWrite(flow_sensor,HIGH);

  attachInterrupt(digitalPinToInterrupt(flow_sensor),flow,RISING);
  currenttime=millis();
  looptime=currenttime;
}

void loop()
{
  currenttime=millis();

  if(currenttime>=(looptime+1000))
  {
    looptime=currenttime;

    if(flow_freq!=0)
    {
      l_minute=(flow_freq/7.5);
      Serial.print("rate:");
      Serial.println(l_minute,DEC);
      
      l_minute=l_minute/60;
      vol=vol+l_minute;
      Serial.print("L/m"); 
      //Serial.print(vol);
      
      Serial.print('\n');
      flow_freq=0;
     
      
     }
  }

}
