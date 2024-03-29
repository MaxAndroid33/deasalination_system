#ifndef __TANKLEVEL_H__
#define __TANKLEVEL_H__

#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#include <Arduino.h>

#endif

// define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

class TankLevel
{
    volatile byte trigPin;
    volatile byte echoPin;
    long duration;
    bool state = false;
    unsigned long current_millis = millis();
    


public:
    TankLevel(byte trigPin, byte echoPin);
    void begin();
    float MaxLevel;
    void setMaxLevelTank(float high);
    float waterLevelInCm();
    float waterLevelInInch();
    float tankLevelPresent();
    long monitor();
    bool status();
    bool isTankEmpty();
};

#endif // __TANKLEVEL_H__