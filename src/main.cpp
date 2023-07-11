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

void handleMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    
    data[len] = 0;
    Serial.print("received==>");
    Serial.println((char*)data);

    char* key = strtok((char*)data, ":");
    char* valueStr = strtok(NULL, " ");
    int value = atoi(valueStr);

    if (strcmp(key, "Angle") == 0) {
        controlServo.setPostion(value);
    }
    if (strcmp(key, "Pump") == 0) {
        inPump.setState(value);
    }
  }
}

void updateMsg(){
    connection.broadcastIP();
    connection.broadcastMsg("TDS:" + String(tds.measure()) +
                            ",Angle:" + String(controlServo.getPostion()) + 
                            ",Pump:" +String(inPump.getState()));
}
void setup()
{   
    Serial.begin(115200);
    connection.setup(handleMessage);

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
    // Serial.print("pump state: ");
    // Serial.println(inPump.getState(),0);
    connection.update();    
    if((millis() - connection.interval) > 2000)
    {   
        updateMsg();

        connection.interval = millis();
    }
}
