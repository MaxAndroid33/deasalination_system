
#include "RelayControl.h"

RelayControl::RelayControl(byte pin, boolean state) : pin(pin), state(state) {}

void RelayControl::begin()
{

    pinMode(this->pin, OUTPUT);
    setState(state);
}

void RelayControl::setState(boolean state)
{
    
    digitalWrite(pin, castStateOutput(state));
}
void RelayControl::setState(int state)
{
    digitalWrite(pin, castStateOutput(state));
}
int RelayControl::getState()
{

    return state;
}

uint8_t RelayControl::castStateOutput(boolean stateValue)
{
    uint8_t value;
    stateValue ? value = 0x1 : value = 0x0; // display only ('0' means ON , '1' means OFF) 
    state = stateValue;

    return value;
}
uint8_t RelayControl::castStateOutput(int stateValue)
{
    uint8_t value;
    stateValue ? value = 0x1 : value = 0x0; // display only ('0' means ON , '1' means OFF) 
    state = boolean(stateValue);
    return value;
}