#include "CurrentSensor.h"

float CurrentSensor::readCurrent()
{
     // put your main code here, to run repeatedly:
  int adc = analogRead(33);
  float voltage = adc*_vcc/4096.0;
   _current = (voltage-1.444)/0.066;

  if (_current < 0) return 0;
  return _current;

} // readCurrent()


CurrentSensor::CurrentSensor(byte pin) : _vin(pin)
{
    _model = 2;
    _vcc = 5;
}

CurrentSensor::CurrentSensor(int model, int vinPin, float vcc)
{

    // -- value
    _vin = vinPin;
    _model = model;
    _vcc = vcc;

}
