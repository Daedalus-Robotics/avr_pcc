#pragma once

#include <Adafruit_NeoPixel.h>

class LedStripComponent {
public:
    explicit LedStripComponent(uint16_t pixelCount);
    void setup();
    void reset();
    void eStop();

    void setColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b);
    void fillColor(uint8_t r, uint8_t g, uint8_t b);

private:
    Adafruit_NeoPixel strip;
};
