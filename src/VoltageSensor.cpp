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
  in_voltage = 0.5 + 1.1 * adc_voltage  - 0.006 *adc_voltage*adc_voltage;

        count = 0;
        sumV = 0;
    }







  
  return in_voltage;
}

int VoltageSensor::battery_level()
{
  float temp_voltage = round(this->in_voltage * 100.0) / 100.0; // take two decimal for example 10.50
  return (temp_voltage <= 10.50) ? 0 :
           (temp_voltage <= 11.31) ? 10 :
           (temp_voltage <= 11.58) ? 20 :
           (temp_voltage <= 11.75) ? 30 :
           (temp_voltage <= 11.90) ? 40 :
           (temp_voltage <= 12.06) ? 50 :
           (temp_voltage <= 12.20) ? 60 :
           (temp_voltage <= 12.32) ? 70 :
           (temp_voltage <= 12.42) ? 80 :
           (temp_voltage <= 12.50) ? 90 : 100;
   
}
