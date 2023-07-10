#ifndef __TDSSENSOR_H__
#define __TDSSENSOR_H__
#include <Arduino.h>

#define VREF 3.3     // analog reference voltage(Volt) of the ADC
#define SUMOFPOINT  30           // sum of sample point

class TdsSensor
{
private:
    float tdsValue = 0;
    float sumTds = 0;
    int count = 0;

public:
    float temperature = 25;  // current temperature for compensation
    volatile byte pin;

    TdsSensor(byte pin,float temperature);
    void begin();
    double measure();    
};





#endif // __TDSSENSOR_H__