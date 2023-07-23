#ifndef VoltageSensor_h
#define VoltageSensor_h

#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#include <Arduino.h>

#endif



class VoltageSensor{

volatile byte pin; // data pin analog voltage
float ref_voltage = 3.3;
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0; 
float in_voltage = 0.0;  // voltage to measure
 // Integer for ADC value
int adc_value = 0;
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;// upper resistor of the sensor
float R2 = 7500.0;  //lower resistor of the sensor

public:
VoltageSensor(int);
void begin();
float voltage_measured();


};


#endif
