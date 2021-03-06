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

#define sample_size 80; //The sampling rate fs at freerun is approximately 8000-9000hz. 
                        //TODO: We need to test our board in order to determine our sample size.

int analogPin = A0;     // Analog input pin. Make sure to keep the input voltage between 0 and 5V.
float analogInput;
float voltage[sample_size] = {0}, voltageCorr[sample_size] = {0};

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
   for(int j = 0; j < sample_size; j++)
   {
      for(int i = 1; i < sample_size; i++)
      {    
        analogInput = analogRead(analogPin);           //read all new data at "free run" state to increase sampling rate and                   
        voltage[i] = analogInput;                      //to make the sampling freq more stable.
      }         
   }
  /*
  voltage[i] = (actual volate / 5V) * 1023 + offset. The arduino does read negitive voltage so we should add 
  analog/digital offset if we need to.
  */
 
  /*
  TODO: Write resampling code here if necessary     
  The sampling frequency should be sable enough as we have set all the readings at free run state.
  if the sampling frequency is still not stable enough we need to apply some resampling code here.
  */
  
  
  //int sample_size = 80;
  
  //autocorr
  for (int i = 0; i < sample_size; i++)
  {
    voltageCorr[i] = AutoCorrelation(voltage, i, sample_size);  
  }                                                            
  
  
  //smoothing
  float volatageCorrSmooth[sample_size] = {0}
  int windowSize = 3; 
  float sum = 0;
  float movingAverage = 0;
  for(int z = 0; z <= (sample_size - windowSize); z++)
  {
     sum = 0.0f;
     for(int q = z; q < z + windowSize; q++){
       if(voltageCorr[q] != 0)
       {
         sum += voltageCorr[q];
       }
       else
       {
         sum += voltageCorr[q-1];
       }
     }
     movingAverage = sum/windowSize;
     volatageCorrSmooth[z] = movingAverage;    
  }
  
  //TODO:Determine the discrete period of signal
  //At around 8000-9000hz sampling rate the period of 550hz signal is 13 samples. We need to test our board to determine the period.
  int period = 13; 
  
  //find max index
  float max_corr_value = 0;
  int max_corr_index = -1;
  for(int j = 0; j < volatageCorrSmooth.size(); j++)
  {
    if((volatageCorrSmooth[i] > max_corr_value) && i > 0.6*period)
    {
      max_corr_index = i;
      max_corr_value = volatageCorrSmooth[i];
    }
  }
  /*
   for (i = 0; i < sample_size; i++) {
    Serial.println(voltage[i]);
   }
   Serial.println("----------------");
   for (i = 0; i < sample_size; i++) {
    Serial.println(voltageCorr[i]);
   }
  */
  Serial.println(max_corr_index);
  /*
  TODO:
  Write the signal detection logic here. if max_corr_index is equal/around the var period then the signal exists. 
  Write the logic when the system is/is not reading the signal here. 
  Write the sending SMS message code here.
  */
}
