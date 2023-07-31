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
    float voltage,sumV = 0 ;
    int count = 0, voltageInt = 0;
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