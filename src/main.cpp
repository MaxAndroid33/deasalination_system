#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>
#include <TdsController.h>
#include <RelayControl.h>
#include <VoltageSensor.h>
#include <CurrentSensor.h>
#include <TankLevel.h>

#define TDS_PIN 35
#define SERVO_PIN 32
#define FLOWMETER_PERMEATE_PIN 27
#define FLOWMETER_CONCENTRATE_PIN 14
#define VOLTAGE_PIN 34
#define CURRENT_PIN 33
#define TANK1_TRIG_PIN 8
#define TANK2_TRIG_PIN 3
#define TANK1_ECHO_PIN 10
#define TANK2_ECHO_PIN 11
#define INNER_PUMP_PIN 13


#define calibrationFactorValue 80.0

const char *ssid = "import4G";
const char *password = "max*!@.77013*!@";

Connection connection(ssid, password);
FlowSensor permeate_flow(FLOWMETER_PERMEATE_PIN, calibrationFactorValue);    // Permeate (purified water)
FlowSensor concentrate_flow(FLOWMETER_CONCENTRATE_PIN, calibrationFactorValue); // concentrate
TdsSensor tds(TDS_PIN, 5);
TdsController control(SERVO_PIN);
VoltageSensor voltageSensor(VOLTAGE_PIN);
CurrentSensor currentSensor(2, CURRENT_PIN, 5);
TankLevel tank1(TANK1_TRIG_PIN,TANK1_ECHO_PIN);
TankLevel tank2(TANK2_TRIG_PIN,TANK2_ECHO_PIN);
RelayControl inPump(INNER_PUMP_PIN, true); // display only ('false' means ON , 'true' means OFF)


unsigned long currentMillis = millis();
unsigned long previousMillis = currentMillis;
unsigned long current_millis = millis();
void increasePulseCountpermeat_flow()
{
    permeate_flow.increasePulseCount();
}
void increasePulseCountSubFlow()
{
    concentrate_flow.increasePulseCount();
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
        float value = atof(valueStr);

        if (strcmp(key, "Angle") == 0)
        {
            control.setPostion(value);
        }
        if (strcmp(key, "Pump") == 0)
        {
            inPump.setState(int(value));
        }
        if (strcmp(key, "setTds") == 0)
        {
            control.setTdsValue(value);
        }
        if (strcmp(key, "KI") == 0)
        {
            control.KI = value;
        }
        if (strcmp(key, "KD") == 0)
        {
            control.KD = value;
        }
        if (strcmp(key, "KP") == 0)
        {
            control.KP = value;
        }
        if (strcmp(key, "reset") == 0)
        {
            connection.resetEsp();
        }
          if (strcmp(key, "maxlevel") == 0)
        {
            tank1.setMaxLevelTank(value);
            tank2.setMaxLevelTank(value);

        }
    }
}

void updateMsg()
{
    connection.broadcastIP();
    connection.broadcastMsg("TDS:" + String(tds.measure()) +
                            ",requireTDS:" + String(control.getTdsRequired()) +
                            ",Angle:" + String(control.getPostion()) +
                            ",temp:" + String(tds.temperatureMeasure()) +
                            ",error:" + String(control.tds_error) +
                            ",Pump:" + String(inPump.getState()) +
                            ",PerFlow:" + String(permeate_flow.literPassed()) +
                            ",ConFlow:" + String(concentrate_flow.literPassed()) +
                            ",TotalFlow:" + String(permeate_flow.literPassed() + concentrate_flow.literPassed()) +
                            ",KI:" + String(control.KI) +
                            ",KD:" + String(control.KD) +
                            ",KP:" + String(control.KP) +
                            ",reset:"+
                            ",tank1:" + String(tank1.tankLevelPresent()) +
                            ",tank2:" + String(tank2.tankLevelPresent()) );
}
void setup()
{
    Serial.begin(115200);
    connection.setup(handleMessage, false);

    permeate_flow.begin();
    concentrate_flow.begin();
    attachInterrupt(digitalPinToInterrupt(permeate_flow.pin), increasePulseCountpermeat_flow, RISING);
    attachInterrupt(digitalPinToInterrupt(concentrate_flow.pin), increasePulseCountSubFlow, FALLING);

    tds.begin();
    inPump.begin();
    control.begin();
    voltageSensor.begin();

    tank1.setMaxLevelTank(50);
    tank1.begin();
    tank2.setMaxLevelTank(50);
    tank2.begin();
}

void loop()
{
    permeate_flow.flowRate();
    concentrate_flow.flowRate();
    tds.measure(); 
    control.moveServo();
    voltageSensor.voltage_measured();
    currentSensor.getCurrent();
    tank1.monitor();
    tank2.monitor();
    connection.update();
    if ((millis() - connection.interval) > 2000)
    {
        updateMsg();

        connection.interval = millis();
    }

    if (millis() - current_millis > 1000)
    {
        control.controlTdsValue(control.getTdsRequired(), tds.measure());
        current_millis = millis();
    }
}
