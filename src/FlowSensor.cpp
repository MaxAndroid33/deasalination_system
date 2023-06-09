#include "FlowSensor.h"

FlowSensor::FlowSensor(int pin, float calibrationFactor) : pin(pin), calibrationFactor(calibrationFactor) {}

void FlowSensor::begin()
{
    pinMode(this->pin, INPUT);

}

double FlowSensor::flowRate()
{
    currentMillis = millis();
  if (currentMillis > (previousMillis + 1000))
  {
     flowRatePerMin =  (pulseCount / (calibrationFactor));
     previousMillis = currentMillis;
     pulseCount = 0;
     totalLiters += flowRatePerMin/60;

  }
    return flowRatePerMin;
}

double FlowSensor::literPassed()
{   

    return totalLiters;
}

void FlowSensor::increasePulseCount()
{
    this->pulseCount++;
}
