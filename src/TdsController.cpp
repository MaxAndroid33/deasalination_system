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
    total_error = 0;
     tds_error =  current_tds_value - setting_tds_Value ;
     tds_I += tds_error;
    // if(abs(tds_error)<100&&abs(tds_error)>5)
    // if(abs(tds_error * KP)>0.5){tds_I = 0;}
    cur_time = millis();
    tds_d = (tds_error-old_tds_error)/(cur_time-pre_time);
    pre_time = cur_time;
    old_tds_error = tds_error;
        total_error = int(round(tds_error * KP+ KI * tds_I + KD * tds_d)) ;
        control_angle += total_error;
        
        if(abs(int(round(KI * tds_I)))>=1){tds_I = 0;}

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
