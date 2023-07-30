#include "CurrentSensor.h"

float CurrentSensor::readCurrent()
{
     // put your main code here, to run repeatedly:
  int tempV = analogRead(33);

    sumV += tempV;
    // sumTds += tdsValue; //1.35*tempTds-0.00025*tempTds*tempTds;
    count++;

    if (count > 1000)
    {


        voltageInt = sumV / count;

   voltage = voltageInt*3.3/4096.0;
   _current = 1.6*(((voltage-1.52)/0.066) - 0.02);

        count = 0;
        sumV = 0;
    }




  if (_current < 0) return 0;
  return _current;

} // readCurrent()


CurrentSensor::CurrentSensor(byte pin) : _vin(pin)
{
    _model = 2;
    _vcc = 3.3;
}

CurrentSensor::CurrentSensor(int model, int vinPin, float vcc)
{

    // -- value
    _vin = vinPin;
    _model = model;
    _vcc = vcc;

}
