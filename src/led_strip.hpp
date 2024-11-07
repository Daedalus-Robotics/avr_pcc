#pragma once

#include <Adafruit_NeoPixel.h>

struct mode_info_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t arg;
};

class LedStripComponent {
public:
    explicit LedStripComponent(uint16_t pixelCount);
    void setup();
    void reset();
    void eStop();

    void setColor(uint8_t n, uint8_t r, uint8_t g, uint8_t b);
    void fillColor(uint8_t r, uint8_t g, uint8_t b);
    void setMode(uint8_t mode, uint8_t r, uint8_t g, uint8_t b, uint8_t arg);

    void update();

private:
    Adafruit_NeoPixel strip;

    uint8_t mode = 0;
    mode_info_t modeInfo;
    uint8_t modeState = 0;

    uint32_t lastUpdate;
};
