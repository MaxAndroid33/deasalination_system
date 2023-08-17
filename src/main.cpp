#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>
#include <TdsController.h>
#include <SelectOutlet.h>
#include <VoltageSensor.h>
#include <CurrentSensor.h>
#include <TankLevel.h>

#define TDS_PIN 35
#define SERVO_PIN 26
#define FLOWMETER_PERMEATE_PIN 14
#define FLOWMETER_CONCENTRATE_PIN 27
#define VOLTAGE_PIN 36
#define CURRENT_PIN 33
#define tankPlant_TRIG_PIN 2
#define tankDrink_TRIG_PIN 25
#define tankPlant_ECHO_PIN 32
#define tankDrink_ECHO_PIN 34
#define TEMPERATURE_SENSOR_PIN 12

#define calibrationFactorValue 80.0

const char *ssid = "import4G";
const char *password = "max*!@.77013*!@";

Connection connection(ssid, password);
FlowSensor permeate_flow(FLOWMETER_PERMEATE_PIN, calibrationFactorValue);       // Permeate (purified water)
FlowSensor concentrate_flow(FLOWMETER_CONCENTRATE_PIN, calibrationFactorValue); // concentrate
TdsSensor tds(TDS_PIN, TEMPERATURE_SENSOR_PIN);
TdsController control(SERVO_PIN);
VoltageSensor voltageSensor(VOLTAGE_PIN);
CurrentSensor currentSensor(2, CURRENT_PIN, 5);
TankLevel tankPlant(tankPlant_TRIG_PIN, tankPlant_ECHO_PIN);
TankLevel tankDrink(tankDrink_TRIG_PIN, tankDrink_ECHO_PIN);
SelectOutlet out;
enum objName
{
    LIVETANK,
    ELECTRICITY,
    PRODUCTION,
    PUMPSANDVALVES,
};
unsigned long currentMillis = millis();
unsigned long previousMillis = currentMillis;
unsigned long current_millis = millis();

void increasePulseCountPermeateFlow();
void increasePulseCountSubFlow();
void handleMessage(void *arg, uint8_t *data, size_t len);
void updateMsg();

void setup()
{
    Serial.begin(115200);
    connection.setup(handleMessage, false);

    permeate_flow.begin();
    concentrate_flow.begin();
    attachInterrupt(digitalPinToInterrupt(permeate_flow.pin), increasePulseCountPermeateFlow, RISING);
    attachInterrupt(digitalPinToInterrupt(concentrate_flow.pin), increasePulseCountSubFlow, FALLING);

    tds.begin();
    out.begin();
    control.begin();
    voltageSensor.begin();

    tankPlant.setMaxLevelTank(28);
    tankPlant.begin();
    tankDrink.setMaxLevelTank(28);
    tankDrink.begin();
}

void loop()
{
    permeate_flow.flowRate();
    concentrate_flow.flowRate();
    control.moveServo();
    voltageSensor.voltage_measured();
    currentSensor.readCurrent();
    tankPlant.monitor();
    tankDrink.monitor();
    tds.measure();
    connection.update();
    if ((millis() - connection.interval) > 2000)
    {
        updateMsg();

        connection.interval = millis();
    }

    if (millis() - current_millis > 500)
    {
        control.controlTdsValue(control.getTdsRequired(), tds.measure());
        current_millis = millis();
    }
}

void increasePulseCountPermeateFlow()
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
            tankPlant.setMaxLevelTank(value);
            tankDrink.setMaxLevelTank(value);
        }
        if (strcmp(key, "plant") == 0)
        {
            out.openValve("plant", boolean(int(value)));
        }
        if (strcmp(key, "drink") == 0)
        {
            out.openValve("drink", boolean(int(value)));
        }
        if (strcmp(key, "innerpump") == 0)
        {
            if ((out.getState("drink") == "on") || (out.getState("plant") == "on")) // one of the valves must be open
                out.runPump("innerpump", boolean(int(value)));
        }
        if (strcmp(key, "plantpump") == 0)
        {
            out.runPump("plantpump", boolean(int(value)));
        }
        if (strcmp(key, "drinkpump") == 0)
        {
            out.runPump("drinkpump", boolean(int(value)));
        }
    }
}

void updateMsg()
{
    connection.broadcastIP();

    String tankData = String(LIVETANK) + "=port:0,level:" + String(tankPlant.tankLevelPresent()) +
                      ",isFilling:" + String(tankPlant.status()) + "|" + String(LIVETANK) + "=port:1,level:" +
                      String(tankDrink.tankLevelPresent()) +
                      ",isFilling:" + String(tankDrink.status()) + "|";

    String productionData = String(PRODUCTION) + "=tds:" + String(tds.measure()) +
                            ",temp:" + String(tds.temperatureMeasure()) +
                            ",preFlow:" + String(permeate_flow.literPassed()) +
                            ",conFlow:" + String(concentrate_flow.literPassed()) + "|";

    String pumpAndValveData = String(PUMPSANDVALVES) + "=innerpump:" + String(out.getBoolState("innerpump")) +
                              ",plantpump:" + String(out.getBoolState("plantpump")) +
                              ",drinkpump:" + String(out.getBoolState("drinkpump")) +
                              ",plantvalve:" + String(out.getBoolState("plant")) +
                              ",drinkvalve:" + String(out.getBoolState("drink")) + "|";

    String electricityData = String(ELECTRICITY) + "=voltageIn:" + String(voltageSensor.voltage_measured()) +
                             ",currentOut:" + String(currentSensor.readCurrent()) +",currentIn:0" +
                             ",batteryLevel:" + String(0) +",duration:1"// still not exist
                             + "|";

    connection.broadcastMsg(tankData + productionData + pumpAndValveData + electricityData);
}
