#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor
{
private:
    int _pin;
    int _rawValue;

public:
    Sensor(int pin);
    ~Sensor();

    void start();

    int readRawValue();
};

#endif