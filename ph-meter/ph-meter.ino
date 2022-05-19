const int analogInPin = A0;         // the pH meter Analog output is connected with the Arduino’s Analog
int sensorValue = 0;                // deklarasi variabel sensorValue = 0
unsigned long int avgValue;         // Store the average value of the sensor feedback
int buf[10],temp;                   // deklarasi variabel buf dan temp
void setup() {
 Serial.begin(9600);                // memulai komunikasi serial dengan baud rate 9600 bps
}
 
void loop() {
 for(int i=0;i<10;i++)              //Get 10 sample value from the sensor for smooth the value
 { 
  buf[i]=analogRead(analogInPin);   // read pin A0
  delay(10);
 }
 for(int i=0;i<9;i++)               //sort the analog from small to large
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
 for(int i=2;i<8;i++)                     //take the average value of 6 center sample
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;  //convert the analog into millivolt
 float phValue = -5.70 * pHVol + 23.22;   //convert the millivolt into pH value
 Serial.print("sensor ph = ");            // menampilkan string sensor ph
 Serial.println(phValue);                 // menampilkan phValue
 
 delay(20);
}
