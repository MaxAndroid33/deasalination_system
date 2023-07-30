#include "VoltageSensor.h"

VoltageSensor::VoltageSensor(int pin) : pin(pin) {}

void VoltageSensor::begin()
{
  pinMode(this->pin, INPUT);
}

float VoltageSensor::voltage_measured()
{

     // put your main code here, to run repeatedly:
  int tempV = analogRead(this->pin);

    sumV += tempV;
    // sumTds += tdsValue; //1.35*tempTds-0.00025*tempTds*tempTds;
    count++;

    if (count > 1000)
    {


        voltageInt = sumV / count;

  adc_voltage = ((R1 + R2) / R2)*(voltageInt * ref_voltage) / 4096.0;
  in_voltage = 1.25 * adc_voltage  - 0.015 *adc_voltage*adc_voltage;

        count = 0;
        sumV = 0;
    }







  
  return in_voltage;
}
