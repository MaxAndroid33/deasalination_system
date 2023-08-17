#ifndef __DATAPATTERN_H__
#define __DATAPATTERN_H__

#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#include <Arduino.h>

#endif

enum objName
{
    LIVETANK,
    POWER,
    PRODUCTION,
    PUMPSANDVALVES,
};
enum singleTanksData{
PORT,LEVEL,ISFILLING
};
enum productionData {
TDS,TEMPERATURE,PREFLOW,CONFLOW
};
enum pumpsAndValvesData{
MAINPUMP=5,PLANTPUMP=4,DRINKPUMP=3,PLANTVALVE=1,DRINKVALVE=0
};
enum powerData{
    VOLTAGEIN,CURRENTIN,CURRENTOUT,DURATION,BATTERYLEVEL,ISBATTERY
};
#endif // __DATAPATTERN_H__