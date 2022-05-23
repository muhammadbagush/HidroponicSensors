#define TdsSensorPin A1                                               // pin analog sensor TDS
#define VREF 5.0                                                      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30                                                    // sum of sample point
int analogBuffer[SCOUNT];                                             // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25, ECvalue = 0;

void setup()
{
    Serial.begin(9600);                                               // memulai komunikasi serial dengan baud rate 9600 bps
    pinMode(TdsSensorPin,INPUT);                                      // pernyataan nama variabel TdsSensorPin sebagai input pin A1
}

void loop()
{
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
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; 
      ECvalue=tdsValue*2;
      Serial.print("TDS Value:");
      Serial.print(tdsValue,0);
      Serial.print("ppm");
      Serial.print("   ");
      Serial.print("EC Value:");
      Serial.println(ECvalue,0);
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
