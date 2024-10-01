#pragma once

#include <Adafruit_PWMServoDriver.h>

class ServoComponent {
public:
    ServoComponent();
    void setup();
    void reset();
    void eStop();

    void update();

    void setEnabled(bool enabled);
    void setMicroseconds(uint8_t num, uint16_t microseconds);

private:
    bool lastConnected;
    uint32_t lastUpdate;
    bool enabled;
    uint16_t microsecondsArr[8];

    Adafruit_PWMServoDriver driver;

    bool checkConnection();
    void sendConfig();
};
