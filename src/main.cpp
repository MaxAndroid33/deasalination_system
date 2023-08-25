#include <FlowSensor.h>
#include <TdsSensor.h>
#include <Connection.h>
#include <TdsController.h>
#include <SelectOutlet.h>
#include <VoltageSensor.h>
#include <CurrentSensor.h>
#include <TankLevel.h>
#include <DataPattern.h>
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

// const char *ssid = "tripleAAA";
// const char *password = "12345678900";
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

unsigned long currentMillis = millis();
unsigned long previousMillis = currentMillis;
unsigned long current_millis = millis();

void increasePulseCountPermeateFlow();
void increasePulseCountSubFlow();
void handleMessage(void *arg, uint8_t *data, size_t len);
void updateMsg();
void updateFastMsg();

String data;

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

    // out.runPump("plantpump", boolean(1));
    // out.runPump("drinkpump", boolean(1));
    // out.runPump("innerpump", boolean(1));
    // out.runPump("plant", boolean(1));
    // out.runPump("drink", boolean(1));
}

void loop()
{
    if (!out.getBoolState("innerpump") && tankDrink.status() && !out.getBoolState("drink"))
        out.runPump("innerpump", true); // (false Means "On" , True means "OFF")
    if (!out.getBoolState("innerpump") && tankPlant.status() && !out.getBoolState("plant"))
        out.runPump("innerpump", true); // (false Means "On" , True means "OFF")

    permeate_flow.flowRate();
    concentrate_flow.flowRate();
    control.moveServo();
    voltageSensor.voltage_measured();
    currentSensor.readCurrent();
    tankPlant.monitor();
    tankDrink.monitor();
    tds.measure();
    connection.update();

    updateFastMsg();

    if ((millis() - connection.interval) > 500)
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
        else if (strcmp(key, "setTds") == 0)
        {
            control.setTdsValue(value);
        }
        else if (strcmp(key, "KI") == 0)
        {
            control.KI = value;
        }
        else if (strcmp(key, "KD") == 0)
        {
            control.KD = value;
        }
        else if (strcmp(key, "KP") == 0)
        {
            control.KP = value;
        }
        else if (strcmp(key, "reset") == 0)
        {
            connection.resetEsp();
        }
        else if (strcmp(key, "maxlevel") == 0)
        {
            tankPlant.setMaxLevelTank(value);
            tankDrink.setMaxLevelTank(value);
        }
        else if (strcmp(key, "plantvalve") == 0)
        {
            out.openValve("plant", boolean(int(value)));
        }
        else if (strcmp(key, "drinkvalve") == 0)
        {
            out.openValve("drink", boolean(int(value)));
        }
        else if (strcmp(key, "mainpump") == 0)
        {
            if ((out.getState("drink") == "on") || (out.getState("plant") == "on")) // one of the valves must be open
                out.runPump("innerpump", boolean(int(value)));
        }
        else if (strcmp(key, "plantpump") == 0)
        {
            out.runPump("plantpump", boolean(int(value)));
        }
        else if (strcmp(key, "drinkpump") == 0)
        {
            out.runPump("drinkpump", boolean(int(value)));
        }
    }
}

void updateMsg()
{
    connection.broadcastIP();

    String tankData = String(LIVETANK) + "=" +
                      String(PORT) + ":1" + "," +
                      String(LEVEL) + ":" + String(tankPlant.tankLevelPresent()) + "," +
                      String(ISFILLING) + ":" + String(tankPlant.status()) + "|" +
                      String(LIVETANK) + "=" + String(PORT) + ":2" + "," +
                      String(LEVEL) + ":" + String(tankDrink.tankLevelPresent()) + "," +
                      String(ISFILLING) + ":" + String(tankDrink.status()) + "|";

    String pumpAndValveData = String(PUMPSANDVALVES) + "=" +
                              String(MAINPUMP) + ":" + String(out.getBoolState(MAINPUMP)) + "," +
                              String(PLANTPUMP) + ":" + String(out.getBoolState(PLANTPUMP)) + "," +
                              String(DRINKPUMP) + ":" + String(out.getBoolState(DRINKPUMP)) + "," +
                              String(PLANTVALVE) + ":" + String(out.getBoolState(PLANTVALVE)) + "," +
                              String(DRINKVALVE) + ":" + String(out.getBoolState(DRINKVALVE)) + "|";

    String powerData = String(POWER) + "=" +
                       String(VOLTAGEIN) + ":" + String(voltageSensor.voltage_measured()) + "," +
                       String(CURRENTIN) + ":0" + "," +
                       String(CURRENTOUT) + ":" + String(currentSensor.readCurrent()) + "," +
                       String(BATTERYLEVEL) + ":" + String(voltageSensor.battery_level()) + "," +
                       String(ISBATTERY) + ":1" + "," +
                       String(DURATION) + ":1" + "|";

    connection.broadcastMsg(tankData + pumpAndValveData + powerData);
}
void updateFastMsg()
{
    static int fastInterval = millis();

    if ((millis() - fastInterval) > 300)
    {

        data = "";
        if (!out.getBoolState(MAINPUMP) || !out.getBoolState(DRINKPUMP))
        {
            data += String(LIVETANK) + "=" +
                    String(PORT) + ":1," +
                    String(LEVEL) + ":" + String(tankPlant.tankLevelPresent()) + "," +
                    String(ISFILLING) + ":" + String(tankPlant.status()) + "|";
        }

        if (!out.getBoolState(MAINPUMP) || !out.getBoolState(PLANTPUMP))
        {
            data += String(LIVETANK) + "=" +
                    String(PORT) + ":2," +
                    String(LEVEL) + ":" + String(tankDrink.tankLevelPresent()) + "," +
                    String(ISFILLING) + ":" + String(tankDrink.status()) + "|";
        }

        if (!out.getBoolState(MAINPUMP))
        {
            data += String(PRODUCTION) + "=" +
                    // String(TDS) + ":" + String((int(millis() / 500) + 10000) % 10000) + "," +
                    String(TDS) + ":" + String(tds.measure()) + "," +
                    String(TEMPERATURE) + ":" + String(25.7) + "," +
                    // String(TEMPERATURE) + ":" + String(0) + "," +
                    String(PREFLOW) + ":" + String(permeate_flow.flowRate()) + "," +
                    String(CONFLOW) + ":" + String(concentrate_flow.flowRate()) + "|";
        }

        if (data.length())
            connection.broadcastMsg(data);
        
        fastInterval = millis();
    }
}
