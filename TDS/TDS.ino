#define OutRelay 3                                                    // Pin 3 Output ke Relay hhhh
#define TdsSensorPin A1                                               // Pin A1 sebagai input jjjjjjjjjjjjj
#define VREF 5.0                                                      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30                                                    // sum of sample point
int analogBuffer[SCOUNT];                                             // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];                                         
int analogBufferIndex = 0,copyIndex = 0;                                                            
float averageVoltage = 0,tdsValue = 0,temperature = 25, ECvalue = 0;  
unsigned long oldtime = 0;                                            // deklarasi waktu sebelumnya
unsigned long hitung = 0;                                             // variabel selisih antara waktu sekarang dengan waktu sebelumnya (oldtime)
int hitungCounting;                                                   // variabel penyimpanan counter inject AB Mix 
int counter = 1;                                                      // variabel counter
float jumlah = 0;                                                     // variabel banyaknya larutan AB Mix yang di larutkan kedalam air (ml)

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(OutRelay, OUTPUT);                                            // pernyataan nama variabel OutRelay sebagai output pin digital 3
pinMode(TdsSensorPin,INPUT);                                          // pernyataan nama variabel TdsSensorPin sebagai input pin analog A1

}

void loop() {
   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)                           //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);      //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
      analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      
      // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; 
      
      //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    
      
      //temperature compensation
      float compensationVolatge=averageVoltage/compensationCoefficient;  
      
      //convert voltage value to tds value
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;    // rumus perhitungan pembacaan sensor tds   
      ECvalue=tdsValue*2;                                                 // rumus perhitungan nilai EC (Electrical Conductivity)
      Serial.print("TDS Value:");                                         // print string tds value
      Serial.print(tdsValue,0);                                           // print nilai variabel sensor tds
      Serial.print("ppm");                                                // print string ppm 
      Serial.print("   ");                                                // print spasi string
      Serial.print("EC Value:");                                          // print EC Value
      Serial.println(ECvalue,0);                                          // print nilai variabel EC
   }

   if(ECvalue <=2800)                                                     // pengkondisian untuk nilai EC kurang dari sama dengan 2800
   {
     hitung = millis() - oldtime;                                         // variabel perhitungan waktu millis dengan set waktu yang sebelumnya (oldtime)
     if( hitung >= 15000)                                                 // pengkondisian variabel perhitungan waktu millist dan oldtime jika lebih dari sama dnegan 15000 ms (15s)
     {
      oldtime = millis();                                                 // variabel set waktu sebelumnya (oldtime)
      digitalWrite(OutRelay, LOW);                                        // mengaktifkan relay
      delay(1000);                                                        // delay ketika relay aktif
      digitalWrite(OutRelay, HIGH);                                       // menonaktifkan relay  
      Serial.println(" ");
      Serial.print("Inject AB mix ke : ");                                // print string
      hitungCounting = counter++;                                         // menyimpan jumlah counter inject AB Mix
      Serial.print(hitungCounting);                                       // print counting inject AB Mix
      jumlah = hitungCounting*1.74;                                       // menghitung banyaknya larutan AB yang dimasukkan kedalam air (ml)     
      Serial.print(" ");
      Serial.print("Jumlah AB mix : ");                                   // menampilkan string Jumlah AB mix
      Serial.print(jumlah);                                               // menampilkan perhitungan jumlah
      Serial.println("ml");                                             
      Serial.println(" ");
     }
   }
   else if (ECvalue > 2800)                                              // jika nilai EC diatas 2500, OutRelay ON
   {
    digitalWrite(OutRelay,HIGH);                                         // menonaktifkan relay 
   }
}
int getMedianNum(int bArray[], int iFilterLen) // median filtering algorithm code
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}
