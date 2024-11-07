#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "Sensor.h"

class HumiditySensor : public Sensor
{
private:
    /* data */
public:
    HumiditySensor(int pin);
    ~HumiditySensor();

    int getNormalizedValue();
};

#endif