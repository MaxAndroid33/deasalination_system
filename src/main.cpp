#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>
#include <TdsController.h>

#define calibrationFactorValue 80.0

Connection connection;

FlowSensor prioriFlow(27, calibrationFactorValue);
FlowSensor subFlow(14, calibrationFactorValue);
TdsSensor tds(35,25);
TdsController controlServo(32);

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
    connection.broadcastMsg("TDS ="+String(tds.finalMeasure())+" Postion ="+String(controlServo.getPostion()));
}
void setup()
{   
    Serial.begin(115200);;

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
    prioriFlow.flowRate();
    subFlow.flowRate();
    Serial.println("=================== Before =============");
    Serial.println(prioriFlow.flowRate());
    Serial.println(prioriFlow.literPassed());
    Serial.println("=================== after =============");

    Serial.println(subFlow.flowRate());
    Serial.println(subFlow.literPassed()); 
   

    Serial.print("TDS Value:");
    Serial.print(tds.finalMeasure(),0);
    Serial.println("ppm");

    controlServo.move();
    controlServo.setPostion(connection.inputPostion);
    connection.update();    
    if((millis() - connection.interval) > 2000)
    {   
        updateMsg();

        connection.interval = millis();
    }
}
