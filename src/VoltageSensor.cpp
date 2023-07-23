#include "VoltageSensor.h"

VoltageSensor::VoltageSensor(int pin) : pin(pin) {}

void VoltageSensor::begin()
{
  pinMode(this->pin, INPUT);
}

float VoltageSensor::voltage_measured()
{
  // Read the Analog Input
  adc_value = analogRead(this->pin);

  // Determine voltage at ADC input
  adc_voltage = (adc_value * ref_voltage) / 4096.0;

  // Calculate voltage at divider input
  in_voltage = adc_voltage * (R1 + R2) / R2;
  return in_voltage;
}
