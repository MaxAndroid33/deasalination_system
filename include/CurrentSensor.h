#ifndef CurrentSensor_h
#define CurrentSensor_h

#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#include <Arduino.h>

#endif

class CurrentSensor
{
public:
    CurrentSensor(byte pin);

    CurrentSensor(int model, int vinPin, float vcc);
    float readCurrent();

private:
    /**
     * read current
     */

    volatile byte pin;
    int _vin, _model;
    float _vcc, _current, _outVoltage;
    static float _sensitivity[];
    static float _quiescent_Output_voltage[];
    static float _magnetic_offset_error[];

    static char *_modelNames[];
};

#endif