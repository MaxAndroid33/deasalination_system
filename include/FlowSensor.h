#ifndef FlowSensor_h
#define FlowSensor_h

#include <Arduino.h>



class FlowSensor{



volatile int pulseCount =0;
float flowRatePerMin =0.0;
float totalLiters =0;
unsigned long currentMillis = millis();
unsigned long previousMillis =currentMillis;
float calibrationFactor ;


public:
volatile byte pin;
FlowSensor(int,float);
void begin();
double flowRate();
double literPassed();
void increasePulseCount();








};




#endif