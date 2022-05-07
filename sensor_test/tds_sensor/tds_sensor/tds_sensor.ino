#define tds_sensor A1
int Adc_value;
unsigned long int avgval;
int buffer_tds[10],temp1;
int i,j;

void tds_sensor_cal()
{
  for(i=0;i<10;i++)
  {
    buffer_tds[i]=analogRead(tds_sensor);
    delay(30);
  }
  for(j=i+1;j<10;j++)
  {
    if(buffer_tds[i]>buffer_tds[j])
    {
      temp1=buffer_tds[i];
      buffer_tds[i]=buffer_tds[j];
      buffer_tds[j]=temp1;
    }
  }

avgval=0;
for(i=2;i<8;i++)
avgval+=buffer_tds[i];

float voltage_value = (float)avgval*5.0/1024.0/6;
float tds=(133.42/voltage_value*voltage_value-255.86*voltage_value*voltage_value+853.39*voltage_value)*0.5;
Serial.print("TDS value");
Serial.print(tds);
Serial.print("PPM"); 
Serial.print('\n');  
    
  
}
void setup()
{
  Serial.begin(9600);
  pinMode(tds_sensor,INPUT);
}

void loop()
{
  tds_sensor_cal();
}
