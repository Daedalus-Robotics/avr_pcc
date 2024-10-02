#pragma once

#include <Adafruit_PWMServoDriver.h>

class ServoComponent {
public:
    ServoComponent();
    void setup();
    void status() const;
    void reset();
    void eStop();

    void update();

    void setEnabled(bool enabled);
    void setMicroseconds(uint8_t num, uint16_t microseconds);

private:
    bool connected;
    uint32_t lastUpdate;
    bool enabled;
    uint16_t microsecondsArr[8];

    Adafruit_PWMServoDriver driver;

    static bool checkConnection();
    void sendConfig();
};
