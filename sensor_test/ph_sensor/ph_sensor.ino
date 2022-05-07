#define ph_sensor A0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
float phValue;
void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);  
  Serial.println("Ready"); 

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(ph_sensor);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue);
  Serial.println(" ");
   
 
}
