#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>
#include <TdsController.h>
#include <InnerPumpControl.h>
#include <VoltageSensor.h>
#include <CurrentSensor.h>

#define calibrationFactorValue 80.0

const char *ssid = "import4G";
const char *password = "max*!@.77013*!@";

Connection connection(ssid, password);

FlowSensor permeate_flow(27, calibrationFactorValue);    // Permeate (purified water)
FlowSensor concentrate_flow(14, calibrationFactorValue); // concentrate

TdsSensor tds(35, 5);
TdsController control(32);
VoltageSensor voltageSensor(34);
CurrentSensor currentSensor(2, 33, 5);

InnerPumpControl inPump(13, true); // display only ('false' means ON , 'true' means OFF)

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
                            ",v1:" + String(permeate_flow.literPassed()) +
                            ",v2:" + String(concentrate_flow.literPassed()) +
                            ",v3:" + String(permeate_flow.literPassed() + concentrate_flow.literPassed()) +
                            ",KI:" + String(control.KI) +
                            ",KD:" + String(control.KD) +
                            ",KP:" + String(control.KP) +
                            ",reset:");
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
}

void loop()
{
    permeate_flow.flowRate();
    concentrate_flow.flowRate();
    // Serial.println("=================== Before =============");
    // Serial.println(permeate_flow.flowRate());
    // Serial.println(permeate_flow.literPassed());
    // Serial.println("=================== after =============");

    // Serial.println(concentrate_flow.flowRate());
    // Serial.println(concentrate_flow.literPassed());

    // Serial.print("TDS Value:");
    // Serial.print(tds.measure(),0);
    tds.measure(); // TODO This reads new voltage not the actual tds value
    // Serial.println("ppm");

    control.moveServo();
    voltageSensor.voltage_measured();
    currentSensor.getCurrent();
    currentSensor.debug();
    // Serial.print("pump state: ");
    // Serial.println(inPump.getState(),0);
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
