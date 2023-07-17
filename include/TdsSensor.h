#ifndef __TDSSENSOR_H__
#define __TDSSENSOR_H__
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define VREF 3.3     // analog reference voltage(Volt) of the ADC
#define SUMOFPOINT  30           // sum of sample point

class TdsSensor
{

OneWire oneWire;
DallasTemperature tempSensor;

private:
int analogBuffer[SUMOFPOINT];     // store the analog value in the array, read from ADC
int analogBufferTemp[SUMOFPOINT];
int analogBufferIndex = 0;
int copyIndex = 0;
float averageVoltage = 0;
float tdsValue = 0;
float sumTds =0 ;
int count =0 ;

     // current temperature for compensation
int getMedianNum(int[] ,int );
public:
    float temperature = 25;  
    volatile byte pin;
    volatile uint8_t tempPin;
float tdsVolt;
    TdsSensor(byte pin,uint8_t tempPin);
    void begin();
    double measure();
    float temperatureMeasure();
   
};





#endif // __TDSSENSOR_H__