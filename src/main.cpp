#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>

#define calibrationFactorValue 80.0

Connection connection;

// FlowSensor prioriFlow(27, calibrationFactorValue);
// FlowSensor subFlow(14, calibrationFactorValue);
// TdsSensor tds(13,24);

// unsigned long currentMillis = millis();
// unsigned long previousMillis =currentMillis;
// void increasePulseCountPrioriFlow()
// {

//     prioriFlow.increasePulseCount();
// }
// void increasePulseCountSubFlow()
// {

//     subFlow.increasePulseCount();
// }

void updateMsg(){
    connection.broadcastIP();
    connection.broadcastMsg(String(random(1,100)));
}
void setup()
{   
    Serial.begin(112500);;

    connection.setup();
    // prioriFlow.begin();
    // subFlow.begin();
    // attachInterrupt(digitalPinToInterrupt(prioriFlow.pin), increasePulseCountPrioriFlow, RISING);
    // attachInterrupt(digitalPinToInterrupt(subFlow.pin), increasePulseCountSubFlow, FALLING);
    // tds.begin();
    

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
    // delay(1000);

    // Serial.print("TDS Value:");
    // Serial.print(tds.finalMeasure(),0);
    // Serial.println("ppm");
    // Serial.print("Temp:");
    // Serial.print(tds.temperature,0);
    // Serial.println("C");
    // currentMillis = millis();
    // if(currentMillis >(previousMillis+15000)){
    //     tds.temperature =25;
    //     previousMillis =currentMillis;
    // }

    connection.update();    
    if((millis() - connection.interval) > 2000)
    {   
        updateMsg();

        connection.interval = millis();
    }
}
