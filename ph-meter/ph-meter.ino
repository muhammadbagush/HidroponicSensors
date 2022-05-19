const int analogInPin = A0;         // mendefinisikan pin A0 sebagai analog input
int sensorValue = 0;                // deklarasi variabel sensorValue = 0
unsigned long int avgValue;         // deklarasi variabel avgValue
int buf[10],temp;                   // deklarasi variabel buf dan temp
void setup() {
 Serial.begin(9600);                // memulai komunikasi serial dengan baud rate 9600 bps
}
 
void loop() {
 for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);   // read pin A0
  delay(10);
 }
 for(int i=0;i<9;i++)
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
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0*6/1024; // rumus pHVol
 float phValue = -5.70 * pHVol + 23.22; // rumus pHValue
 Serial.print("sensor ph = "); // menampilkan string sensor ph
 Serial.println(phValue); // menampilkan phValue
 
 delay(20);
}
