
#include "SelectOutlet.h"

void SelectOutlet::begin()
{
    inPump.begin();
    drinkValve.begin();
    drinkPumpOut.begin();
    plantValve.begin();
    plantPumpOut.begin();
}

String SelectOutlet::openValve(String type,boolean status)
{
    switch (ConvertType(type))
    {
    case drink:
        drinkValve.setState(status);
        plantValve.setState(!status);
        return type + String(":") + String(status);
        break;
    case plant:
        plantValve.setState(status);
        drinkValve.setState(!status);
        return type + String(":") + String(status);
        break;
    default:
        break;
    }
    return "NULL";
}

String SelectOutlet::runPump(String type, boolean status)
{

    switch (ConvertType(type))
    {
    case drinkpump:
        drinkPumpOut.setState(status);
        return type + String(":") + String(status);
        break;
    case plantpump:
        plantPumpOut.setState(status);
        return type + String(":") + String(status);
        break;
    case innerpump:
        inPump.setState(status);
        return type + String(":") + String(status);
        break;
    default:
        break;
    }
    return "NULL";
}

String SelectOutlet::getState(String type)
{
    switch (ConvertType(type))
    {
    case drink:
        return toString(drinkValve.getState());
        break;
    case plant:
        return toString(plantValve.getState());
        break;
    case plantpump:
        return toString(plantPumpOut.getState());
        break;
    case drinkpump:
        return toString(drinkPumpOut.getState());
        break;
    case innerpump:
        return toString(inPump.getState());
        break;
    default:
    return "NULL";
        break;
    }
    
}
int SelectOutlet::getBoolState(String type)
{
    switch (ConvertType(type))
    {
    case drink:
        return (drinkValve.getState());
        break;
    case plant:
        return (plantValve.getState());
        break;
    case plantpump:
        return (plantPumpOut.getState());
        break;
    case drinkpump:
        return (drinkPumpOut.getState());
        break;
    case innerpump:
        return (inPump.getState());
        break;
    default:
    return 3000;
        break;
    }
    
}
int SelectOutlet::getBoolState(int type)
{
    switch (type)
    {
    case drink:
        return (drinkValve.getState());
        break;
    case plant:
        return (plantValve.getState());
        break;
    case plantpump:
        return (plantPumpOut.getState());
        break;
    case drinkpump:
        return (drinkPumpOut.getState());
        break;
    case innerpump:
        return (inPump.getState());
        break;
    default:
    return 3000;
        break;
    }
    
}

int SelectOutlet::ConvertType(String type)
{
    if (strcmp(type.c_str(), "drink") == 0)
        return drink;
    else if (strcmp(type.c_str(), "plant") == 0)
        return plant;
    else if (strcmp(type.c_str(), "drinkpump") == 0)
        return drinkpump;
    else if (strcmp(type.c_str(), "plantpump") == 0)
        return plantpump;
    else if (strcmp(type.c_str(), "innerpump") == 0)
        return innerpump;
    else
        return 400;
}

String SelectOutlet::toString(int status)
{
    switch (status)
    {
    case 1:
        return "off";
        break;
    case 0:
    default:
        return "on";
        break;
    }
}
