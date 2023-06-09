#ifndef __TDSSENSOR_H__
#define __TDSSENSOR_H__
#include <Arduino.h>

#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SUMOFPOINT  30           // sum of sample point
class TdsSensor
{
private:
int analogBuffer[SUMOFPOINT];     // store the analog value in the array, read from ADC
int analogBufferTemp[SUMOFPOINT];
int analogBufferIndex = 0;
int copyIndex = 0;
float averageVoltage = 0;
float tdsValue = 0;
     // current temperature for compensation
int getMedianNum(int[] ,int );
public:
    float temperature = 25;  
    volatile byte pin;
    TdsSensor(byte pin,float temperature);
    void begin();
    double finalMeasure();



    
};





#endif // __TDSSENSOR_H__