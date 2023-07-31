#ifndef __SELECTOUTLET_H__
#define __SELECTOUTLET_H__

#ifndef __ARDUINO_H__
#define __ARDUINO_H__
#include <Arduino.h>

#endif
#define INNER_PUMP_PIN 13
#define DRINK_VALVE_PIN 25
#define DRINK_PUMP_PIN 26
#define PLANT_VALVE_PIN 18
#define PLANT_PUMP_PIN 19

#ifndef RelayControl_h
#define RelayControl_h
#include "RelayControl.h"

#endif

class SelectOutlet
{
    enum 
    {
        drink = 0,
        plant = 1,
        drinkpump =3,
        plantpump =4,
        innerpump =5
        
    };
    
     

    RelayControl inPump = RelayControl(INNER_PUMP_PIN, true); // display only ('false' means ON , 'true' means OFF)
    RelayControl drinkValve = RelayControl(DRINK_VALVE_PIN, true);
    RelayControl drinkPumpOut = RelayControl(DRINK_PUMP_PIN, true);
    RelayControl plantValve = RelayControl(PLANT_VALVE_PIN, true);
    RelayControl plantPumpOut = RelayControl(PLANT_PUMP_PIN, true);
   
    int ConvertType(String type);
    String toString(int status);

public:
    void begin();
    String openValve(String type,boolean status);
    String runPump(String type,boolean status);
    String getState(String type);
     

};

#endif // __SELECTOUTLET_H__