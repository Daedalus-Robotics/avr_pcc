#pragma once

#include <Adafruit_NeoPixel.h>

class OnboardLedComponent {
public:
    OnboardLedComponent();
    void setup();
    void reset();
    void eStop();

    void setColor(uint8_t r, uint8_t g, uint8_t b);

private:
    Adafruit_NeoPixel onboard;
};
