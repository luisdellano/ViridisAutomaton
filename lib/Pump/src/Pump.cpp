#include "Pump.h"
#include <Arduino.h>

Pump::Pump(int relayPIN) : _relayPIN(relayPIN), _pumpState(false) {}

Pump::~Pump() {}

void Pump::start()
{
    pinMode(_relayPIN, OUTPUT);
    turnOff();
}

void Pump::turnOn()
{
    digitalWrite(_relayPIN, HIGH);
    _pumpState = true;
}

void Pump::turnOff()
{
    digitalWrite(_relayPIN, LOW);
    _pumpState = false;
}

bool Pump::pumpState()
{
    return _pumpState;
}
