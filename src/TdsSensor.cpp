#include "TdsSensor.h"

TdsSensor::TdsSensor(byte pin, uint8_t tempPin) : pin(pin), tempPin(tempPin) {}

void TdsSensor::begin()
{
    oneWire.begin(tempPin);
    tempSensor.setOneWire(&oneWire);

    tempSensor.begin();
    pinMode(pin, INPUT);
}

double TdsSensor::measure()
{

    float tempVolt = analogRead(this->pin); // read the analog value and store into the buffer

    sumTds += tempVolt;
    // sumTds += tdsValue; //1.35*tempTds-0.00025*tempTds*tempTds;
    count++;

    if (count > 2000)
    {

        tdsVolt = sumTds / count;
        float tdsVolt_temp = tdsVolt * (float)VREF / 4096.0;
        float compensationCoefficient = 1.0 + 0.02 * (temperatureMeasure() - 25.0);
        // // // temperature compensation
        float compensationVoltage = tdsVolt_temp / compensationCoefficient;

        // // // convert voltage value to tds value
        float tempTds = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
        tdsValue = a * tempTds + b;
        count = 0;
        sumTds = 0;
    }
    return tdsValue;
}

float TdsSensor::temperatureMeasure()
{
    tempSensor.requestTemperatures();
    float tempC = tempSensor.getTempCByIndex(0);
    if (int(tempC) == -127)
    {
        return temperature;
    }
    else
    {
        temperature = tempC;
        return temperature;
    }
}
// median filtering algorithm
int TdsSensor::getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
        bTab[i] = bArray[i];
    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++)
    {
        for (i = 0; i < iFilterLen - j - 1; i++)
        {
            if (bTab[i] > bTab[i + 1])
            {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }
    if ((iFilterLen & 1) > 0)
    {
        bTemp = bTab[(iFilterLen - 1) / 2];
    }
    else
    {
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    }
    return bTemp;
}