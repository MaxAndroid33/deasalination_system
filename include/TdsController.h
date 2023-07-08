#ifndef __TDSCONTROLLER_H__
#define __TDSCONTROLLER_H__

#include <Arduino.h>
#include <Servo.h>


class TdsController{
Servo myServo;  // create servo object to control a servo
volatile byte pin;
int pos = 170;    // variable to store the servo position
public:
float error(float requireTds, float sensorTds);
void setPostion(int postion);
void begin();
TdsController(byte pin); 
void move();
int getPostion();


};




#endif // __TDStCONTROLLER_H__