#ifndef __TDSCONTROLLER_H__
#define __TDSCONTROLLER_H__

#include <Arduino.h>
#include <Servo.h>

class TdsController
{

    Servo myServo; // create servo object to control a servo
    volatile byte pin;
    int pos = 100; // variable to store the servo position
    float set_tds_value = 500;

    double old_tds_error = 0;
    int control_angle=100;
    unsigned long cur_time = millis();
    unsigned long pre_time = cur_time;

public:
    double tds_error = 0;
    double tds_I = 0;
    double tds_d = 0;
    float KI = 0 ,KP = 0.08 ,KD=0;

    void setPostion(int postion);
    void begin();
    TdsController(byte pin);
    void moveServo();
    int getPostion();
    void controlTdsValue(double setting_tds_Value, double current_tds_value);
    void setTdsValue(float value);
    float getTdsRequired();
    
};

#endif // __TDStCONTROLLER_H__