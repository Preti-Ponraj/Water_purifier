#include<LiquidCrystal.h>

#define ph_sensor A0
#define tds_sensor A1
#define flow_sensor 2
#define valve1 32
#define valve2 33
#define valve3 34
#define valve4 35
#define valve5 36
#define motor 37
#define rs 41
#define en 43
#define d4 45
#define d5 47
#define d6 49
#define d7 51

volatile int flow_freq;
float vol=0.0,l_minute;
unsigned long currenttime,looptime;
float conductivity;

int Adc_value;
unsigned long int avgval;
int buffer_tds[10],temp1;
int i,j;
float tds;


unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
float phValue;



LiquidCrystal lcd(41,43,45,47,49,51); //(rs,enable,D4,D5,D6,D7)


void flow()
{
  flow_freq++;
}



void setup() 
{
  // put your setup code here, to run once:  
  Serial.begin(9600);  
  Serial.println("Ready"); 
  lcd.begin(16,2);

  pinMode(flow_sensor,INPUT);
  pinMode(tds_sensor,INPUT);
  digitalWrite(flow_sensor,HIGH);

  attachInterrupt(digitalPinToInterrupt(flow_sensor),flow,RISING);
  currenttime=millis();
  looptime=currenttime;
}

float tds_sensor_cal()
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
Serial.print(tds);
return tds;
}

float flow_sensor_cal()
{
  currenttime=millis();

  if(currenttime>=(looptime+1000))
  {
    looptime=currenttime;

    if(flow_freq!=0)
    {
      l_minute=(flow_freq/7.5);
      //lcd.clear();
      //lcd.setCursor(0,0);
      
      //Serial.println(l_minute,DEC);
      
      l_minute=l_minute/60;
      vol=vol+l_minute;
      //lcd.print(vol);
      //lcd.print("L/m"); 
      //Serial.print(vol);
      
      //Serial.print('\n');
      flow_freq=0;
      return vol;
      
     }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("hello");
  vol=flow_sensor_cal();
  tds=tds_sensor_cal();

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
  //Serial.println(" ");

 
  conductivity=tds/0.67;
  lcd.clear();
  lcd.blink();
  //lcd.print("rate:");
  lcd.setCursor(0,0);
  lcd.print("vol:");
  lcd.setCursor(0,1);
  lcd.print(vol,1);
  
  //lcd.print("L/M");
  delay(2000);
  lcd.setCursor(6,0);
  lcd.print("pH:");
  lcd.setCursor(6,1);
  lcd.print(phValue,1);
  delay(2000);
  //lcd.clear();
  //lcd.blink();
  //lcd.home();
  
  //lcd.print("PPM");
  delay(2000);
  lcd.setCursor(11,0);
  lcd.print("TDS:");
  lcd.setCursor(11,1);
  lcd.print(tds,1);
  delay(2000);
  
  
  
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0);
  if(tds<300.0)
 {
  
  lcd.print("PURE WATER");
 }
 else
 {
  
  lcd.print("IMPURE WATER");
 }

  

  if(phValue>=6.5 && phValue<=8.5 && (digitalRead(valve2)==0))
  {
    digitalWrite(valve1,HIGH);
    digitalWrite(valve2,LOW);
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("valve1 is open");
    delay(2000);
  }
  
  else
  {
    digitalWrite(valve2,HIGH); 
    delay(2000);
    
  }
  if(digitalRead(valve2)==HIGH)
  {
    delay(5000);
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("valve2 is open");
    
    if(phValue<6.5)
    {
    delay(2000);
    digitalWrite(valve3,HIGH);
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("valve3 is open");
    }
    if(phValue>8.5)
    {
    delay(2000);
    digitalWrite(valve4,HIGH);
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("valve4 is open");
    delay(5000);
    }
    delay(5000);
    if(digitalRead(valve3)==HIGH || digitalRead(valve4)==HIGH)
    {
    delay(2000);
    digitalWrite(motor,HIGH);
    delay(20000); 
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("chemical process");
    delay(20000);
    digitalWrite(valve5,HIGH);
    lcd.clear();
    lcd.home();
    lcd.setCursor(0,0);
    lcd.print("valve5 is open");
    delay(2000);
    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("organic process");
    delay(20000);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cond");
    lcd.setCursor(0,1);
    lcd.print(conductivity,2);
    lcd.setCursor(8,0);
    lcd.print("Turb");
    lcd.setCursor(8,1);
    lcd.print("5");
    delay(20000);
    
    }
  }
 
 
}
