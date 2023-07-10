#include "TdsSensor.h"

TdsSensor::TdsSensor(byte pin, float temperature) : pin(pin), temperature(temperature) {}

void TdsSensor::begin()
{
    pinMode(pin, INPUT);
}

double TdsSensor::measure()
{
    float tempVolt = analogRead(this->pin); // read the analog value and store into the buffer

    // float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    // // // temperature compensation
    // float compensationVoltage = tempVolt / compensationCoefficient;

    // // // convert voltage value to tds value
    // float tempTds = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
    sumTds += tempVolt;
    // sumTds += tdsValue; //1.35*tempTds-0.00025*tempTds*tempTds;
    count++;

    if(count > 2000)
    {
        tdsValue = sumTds / count;
        count = 0;
        sumTds = 0;
    }
    return tdsValue;
}