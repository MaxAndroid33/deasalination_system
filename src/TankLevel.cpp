
#include "TankLevel.h"

TankLevel::TankLevel(byte trigPin, byte echoPin) : trigPin(trigPin), echoPin(echoPin) {}

void TankLevel::begin()
{
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void TankLevel::setMaxLevelTank(float high){ MaxLevel =high;}

float TankLevel::waterLevelInCm()
{
    return (duration * SOUND_SPEED/2)-1 ;
}

float TankLevel::waterLevelInInch()
{
    return waterLevelInCm() * CM_TO_INCH;
}

float TankLevel::tankLevelPresent()
{   
    float temp_level = ((MaxLevel - waterLevelInCm())/MaxLevel)* 100;

    return temp_level>=0? temp_level:0;
}

long TankLevel::monitor()
{
    // Clears the trigPin
//   digitalWrite(trigPin, LOW);
//    delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  if (millis() - current_millis > 2000)
    {
          digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
        current_millis = millis();
    }

    return duration;
}

bool TankLevel::status()
{   
    if(tankLevelPresent() > 99){
        state = true;
    }
    if(tankLevelPresent() < 50 ){
        state = false;
    }
    
    return state; //Tank if Full return 'True' ,if Low retturn 'False'
    
}

bool TankLevel::isTankEmpty()
{
    if(tankLevelPresent() <5) return true;
    else return false;
}
