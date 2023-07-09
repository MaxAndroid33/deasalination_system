
#include "InnerPumpControl.h"

InnerPumpControl::InnerPumpControl(byte pin ,boolean state ) :pin(pin),state(state) {}


void InnerPumpControl::begin() {

    pinMode(this->pin, OUTPUT);
}

void InnerPumpControl::setState(boolean state) {

    
    digitalWrite(pin,castStateOutput(state));
}
void InnerPumpControl::setState(int state) {
    digitalWrite(pin,castStateOutput(state));
}
int InnerPumpControl::getState() {
    
    return state;
}

uint8_t InnerPumpControl::castStateOutput(boolean stateValue)
{     uint8_t value;  
     stateValue ?value = 0x1:value = 0x0;
    
    return value;
}
uint8_t InnerPumpControl::castStateOutput(int stateValue)
{     uint8_t value;  
     stateValue ?value = 0x1:value = 0x0;
    
    return value;
}