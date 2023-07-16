#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>
#include <TdsController.h>
#include <InnerPumpControl.h>

#define calibrationFactorValue 80.0

const char *ssid = "import4G";
const char *password = "max*!@.77013*!@";

Connection connection(ssid, password);

FlowSensor prioriFlow(27, calibrationFactorValue);
FlowSensor subFlow(14, calibrationFactorValue);
double set_tds_value = 500;
double tds_error = 0;
double old_tds_error = 0;
double tds_I = 0;
double tds_d = 0;
int control_angle = 55;

TdsSensor tds(35, 5);
TdsController controlServo(32);

InnerPumpControl inPump(13, true); // display only ('false' means ON , 'true' means OFF)

unsigned long currentMillis = millis();
unsigned long previousMillis = currentMillis;

 unsigned long cur_time = millis();
 unsigned long pre_time = cur_time;
/// tds control function
void tds_control(double setting_tds_Value, double current_tds_value)
{
    tds_error =  current_tds_value - setting_tds_Value ;
    tds_I += tds_error;
    cur_time = millis();
    tds_d = (tds_error-old_tds_error)/(cur_time-pre_time);
    pre_time = cur_time;
    old_tds_error = tds_error;

        control_angle += int(tds_error * 0.08) + 0.00002 * tds_I + 20 * tds_d;
        if (control_angle <= 0)
        {
            control_angle = 0;
        }
        else if (control_angle >= 100)
        {
            control_angle = 100;
        }
        
        
            controlServo.setPostion(control_angle);
        
    
}

void increasePulseCountPrioriFlow()
{
    prioriFlow.increasePulseCount();
}
void increasePulseCountSubFlow()
{
    subFlow.increasePulseCount();
}

void handleMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {

        data[len] = 0;
        Serial.print("received==>");
        Serial.println((char *)data);

        char *key = strtok((char *)data, ":");
        char *valueStr = strtok(NULL, " ");
        int value = atoi(valueStr);

        if (strcmp(key, "Angle") == 0)
        {
            controlServo.setPostion(value);
        }
        if (strcmp(key, "Pump") == 0)
        {
            inPump.setState(value);
        }
        if (strcmp(key, "tdsv") == 0)
        {
            set_tds_value = value;
        }
    }
}

void updateMsg()
{
    connection.broadcastIP();
    connection.broadcastMsg("TDS:" + String(tds.measure()) +
                            ",temp:" + String(tds_error) +
                            ",Angle:" + String(controlServo.getPostion()) +
                            ",Pump:" + String(inPump.getState()));
}
void setup()
{
    Serial.begin(115200);
    connection.setup(handleMessage, false);

    prioriFlow.begin();
    subFlow.begin();
    attachInterrupt(digitalPinToInterrupt(prioriFlow.pin), increasePulseCountPrioriFlow, RISING);
    attachInterrupt(digitalPinToInterrupt(subFlow.pin), increasePulseCountSubFlow, FALLING);

    tds.begin();
    inPump.begin();
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
    tds.measure(); // TODO This reads new voltage not the actual tds value
    // Serial.println("ppm");

    controlServo.move();
    // Serial.print("pump state: ");
    // Serial.println(inPump.getState(),0);
    connection.update();
    if ((millis() - connection.interval) > 2000)
    {
        updateMsg();

        connection.interval = millis();
    }

    
    static unsigned long current_millis = millis();
    if( millis() - current_millis > 1000){
        tds_control(set_tds_value,tds.measure());
        current_millis = millis();

    }


}
