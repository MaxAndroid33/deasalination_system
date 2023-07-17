#include "TdsController.h"


TdsController::TdsController(byte pin) : pin(pin) {}



void TdsController::moveServo()
{
     myServo.write(pos); 
            
}

int TdsController::getPostion()
{
    return pos;
}

void TdsController::controlTdsValue(double setting_tds_Value, double current_tds_value)
{
     tds_error =  current_tds_value - setting_tds_Value ;
    tds_I += tds_error;
    cur_time = millis();
    tds_d = (tds_error-old_tds_error)/(cur_time-pre_time);
    pre_time = cur_time;
    old_tds_error = tds_error;

        control_angle += int(tds_error * 0.08) + 0.00002 * tds_I + 20 * tds_d;
        if (control_angle <= 0)
        {
            control_angle = 0;
        }
        else if (control_angle >= 100)
        {
            control_angle = 100;
        }
        
        
        setPostion(control_angle);
}

void TdsController::setTdsValue(float value)
{
    set_tds_value =value;
}

float TdsController::getTdsRequired()
{
    return set_tds_value;
}

void TdsController::setPostion(int postion)
{
    pos =postion;
}

void TdsController::begin()
{
    myServo.attach(pin);  // attaches the servo on pin 9 to the servo object
}
