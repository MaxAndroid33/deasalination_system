
#include "InnerPumpControl.h"

InnerPumpControl::InnerPumpControl(byte pin, boolean state) : pin(pin), state(state) {}

void InnerPumpControl::begin()
{

    pinMode(this->pin, OUTPUT);
    setState(state);
}

void InnerPumpControl::setState(boolean state)
{
    
    digitalWrite(pin, castStateOutput(state));
}
void InnerPumpControl::setState(int state)
{
    digitalWrite(pin, castStateOutput(state));
}
int InnerPumpControl::getState()
{

    return state;
}

uint8_t InnerPumpControl::castStateOutput(boolean stateValue)
{
    uint8_t value;
    stateValue ? value = 0x1 : value = 0x0; // display only ('0' means ON , '1' means OFF) 
    state = stateValue;

    return value;
}
uint8_t InnerPumpControl::castStateOutput(int stateValue)
{
    uint8_t value;
    stateValue ? value = 0x1 : value = 0x0; // display only ('0' means ON , '1' means OFF) 
    state = boolean(stateValue);
    return value;
}