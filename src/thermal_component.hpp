#pragma once

#include <Adafruit_AMG88xx.h>
#include <Arduino.h>

class ThermalComponent {
public:
    ThermalComponent();
    void setup();
    void reset();
    void eStop();

    void update();

private:
    bool isConnected;
    uint32_t lastUpdate;
    Adafruit_AMG88xx amg;
};
