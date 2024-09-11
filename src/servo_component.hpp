#pragma once

#include <Adafruit_PWMServoDriver.h>

class ServoComponent {
public:
    ServoComponent();
    void setup();
    void reset();
    void eStop();

    void setEnabled(bool enabled);
    void setMicroseconds(uint8_t num, uint16_t microseconds);

private:
    Adafruit_PWMServoDriver driver;
};
