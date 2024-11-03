#ifndef PUMP_H
#define PUMP_H

class Pump
{
private:
    /* data */
    int _relayPIN;
    bool _pumpState;

public:
    Pump(int relayPIN);

    ~Pump();

    void start();

    void turnOn();

    void turnOff();

    bool pumpState();
};

#endif