#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(int pin) : Sensor(pin) {}

HumiditySensor::~HumiditySensor()
{
}

int HumiditySensor::getNormalizedValue()
{
    int humidity = map(readRawValue(), 4095, 0, 0, 100); // Normaliza para 0 a 100
    return constrain(humidity, 0, 100);
}
