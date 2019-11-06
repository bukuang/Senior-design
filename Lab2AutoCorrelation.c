// A digital frequency selective filter

// A. Kruger, 2019

//

// The following defines are used for setting and clearing register bits

// on the Arduino processor. Low-level stuff: leave alone.

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
int analogPin = A0;     // Analog input pin. Make sure to keep between 0 and 5V.
int segment = 80;
float analogInput;
float voltage[80] = {0}, voltageCorr[80] = {0};

float AutoCorrelation(float* data, int m, int N)
{
  float r = 0.0f;
      for(int i = m; i < N; i++)
      {
        r += data[i] * data[i-m];
      }
  return r;
}

void setup()
{
   sbi(ADCSRA,ADPS2);     // Next three lines make the ADC run faster
   cbi(ADCSRA,ADPS1);
   cbi(ADCSRA,ADPS0);
   Serial.begin(9600);

}

void loop()
{
   int i =0;
   int j = 0;
   for(j = 0; j < segment; j++)
   {
    for(i = 1; i < segment; i++){                              
      voltage[i] = analogInput;  
      }
     
   }
 
  float max_corr_value = -1;
  int max_corr_index = 0;
  int sampleSize = 80;
  for (int i = 0; i < sampleSize; i++)
  {
    voltageCorr[i] = AutoCorrelation(voltage, i, 80);
  }
  //smoothing
  float volatageCorrSmooth[80] = {0}
  int windowSize = 3; 
  float sum = 0;
  float movingAverage = 0;
  for(int z=0; z<=(sampleSize-windowSize); z++){
     sum = 0.0;
     for(int q=z; q<z+windowSize; q++){
       if(voltageCorr[q]!=0){
         sum += voltageCorr[q];
       }
       else
       {
         sum+=voltageCorr[q-1];
       }
       
     }
     movingAverage = sum/windowSize;
     volatageCorrSmooth[z] = movingAverage;
      
  }
  //find max
  for(int j=0; j<volatageCorrSmooth.size(); j++){
        if((volatageCorrSmooth[i] > max_corr_value) && i > 9)
    {
      max_corr_index = i;
      max_corr_value = volatageCorrSmooth[i];
    }
  }
  /*
   for (i = 0; i < segment; i++) {
    Serial.println(voltage[i]);
   }
   Serial.println("----------------");
   for (i = 0; i < segment; i++) {
    Serial.println(voltageCorr[i]);
   }
  */
  Serial.println(max_corr_index);
  
 
}
