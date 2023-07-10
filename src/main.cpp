#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>
#include <TdsController.h>
#include <InnerPumpControl.h>

#define calibrationFactorValue 80.0

const char* ssid = "YourAPSSID";
const char* password = "12345678";


Connection connection(ssid, password);

FlowSensor prioriFlow(27, calibrationFactorValue);
FlowSensor subFlow(14, calibrationFactorValue);

TdsSensor tds(35,25);
TdsController controlServo(32);

InnerPumpControl inPump(13,true); // display only ('false' means ON , 'true' means OFF) 

unsigned long currentMillis = millis();
unsigned long previousMillis =currentMillis;

void increasePulseCountPrioriFlow()
{
    prioriFlow.increasePulseCount();
}
void increasePulseCountSubFlow()
{
    subFlow.increasePulseCount();
}

void updateMsg(){
    connection.broadcastIP();
    connection.broadcastMsg("VolTDS: " + String(tds.measure()) +
                            ", Postion: " + String(controlServo.getPostion()) + 
                            ", pumpState: " +String(inPump.getState()));
}
void setup()
{   
    Serial.begin(115200);
    connection.setup();

    prioriFlow.begin();
    subFlow.begin();
    attachInterrupt(digitalPinToInterrupt(prioriFlow.pin), increasePulseCountPrioriFlow, RISING);
    attachInterrupt(digitalPinToInterrupt(subFlow.pin), increasePulseCountSubFlow, FALLING);

    tds.begin();
    controlServo.begin();
}

void loop()
{
    // prioriFlow.flowRate();
    // subFlow.flowRate();
    // Serial.println("=================== Before =============");
    // Serial.println(prioriFlow.flowRate());
    // Serial.println(prioriFlow.literPassed());
    // Serial.println("=================== after =============");

    // Serial.println(subFlow.flowRate());
    // Serial.println(subFlow.literPassed()); 
   

    // Serial.print("TDS Value:");
    // Serial.print(tds.measure(),0);
    tds.measure(); //TODO This reads new voltage not the actual tds value
    // Serial.println("ppm");

    controlServo.move();
    controlServo.setPostion(connection.inputPosition);
    inPump.setState(connection.pumpState); // display only ('0' means ON , '1' means OFF) 
    // Serial.print("pump state: ");
    // Serial.println(inPump.getState(),0);
    connection.update();    
    if((millis() - connection.interval) > 2000)
    {   
        updateMsg();

        connection.interval = millis();
    }
}
