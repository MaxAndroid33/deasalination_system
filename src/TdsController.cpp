#include "TdsController.h"


TdsController::TdsController(byte pin) : pin(pin) {}



void TdsController::move()
{
     myServo.write(pos); 
            
}

int TdsController::getPostion()
{
    return pos;
}

void TdsController::setPostion(int postion)
{
    pos =postion;
}

void TdsController::begin()
{
    myServo.attach(pin);  // attaches the servo on pin 9 to the servo object
}
