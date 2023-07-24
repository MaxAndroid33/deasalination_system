#ifndef __INNERPUMPCONTROL_H__
#define __INNERPUMPCONTROL_H__
#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#include <Arduino.h>

#endif

class RelayControl{

uint8_t castStateOutput(boolean stateValue);
uint8_t castStateOutput(int stateValue);
public:
boolean state;
byte pin;
RelayControl(byte pin,boolean state);
void setState(boolean state);
void setState(int state);

int getState();
void begin();


};


#endif // __INNERPUMPCONTROL_H__