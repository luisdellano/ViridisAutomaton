#include "Sensor.h"

Sensor::Sensor(int pin) : _pin(pin), _rawValue(0) {}

void Sensor::start()
{
    pinMode(_pin, INPUT);
}

int Sensor::readRawValue()
{
    _rawValue = analogRead(_pin); // Lê o valor analógico do sensor
    return _rawValue;
}
