#include "led_strip.hpp"

#include "comm.hpp"
#include "topics.h"

#define REFRESH_RATE 6
#define REFRESH_INTERVAL (1000 / REFRESH_RATE)
#define CYCLE_INTERVAL (REFRESH_INTERVAL / 2)

LedStripComponent::LedStripComponent(uint16_t pixelCount) : strip(pixelCount, 12, NEO_GRB),
                                                            modeInfo(), lastUpdate(millis()) {
    pinMode(10, OUTPUT);
}

void LedStripComponent::setup() {
    digitalWrite(10, HIGH);

    strip.begin();
    strip.fill(0);
    strip.setBrightness(255);
    strip.show();

    registerTopic(TOPIC_LED_STRIP_SET, [this](const message_t *message) -> void {
        setColor(message->data[0], message->data[1], message->data[2], message->data[3]);
    });
    registerTopic(TOPIC_LED_STRIP_FILL, [this](const message_t *message) -> void {
        fillColor(message->data[0], message->data[1], message->data[2]);
    });
    registerTopic(TOPIC_LED_STRIP_MODE, [this](const message_t *message) -> void {
        setMode(message->data[0], message->data[1], message->data[2], message->data[3], message->data[4]);
    });
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void LedStripComponent::reset() { // NOLINT(*-convert-member-functions-to-static)
    digitalWrite(10, LOW);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void LedStripComponent::eStop() { // NOLINT(*-convert-member-functions-to-static)
    digitalWrite(10, LOW);
}

void LedStripComponent::setColor(const uint8_t n, const uint8_t r, const uint8_t g, const uint8_t b) {
    if (mode == 0) {
        strip.setPixelColor(n, Adafruit_NeoPixel::Color(r, g, b));
        strip.show();
    }
}

void LedStripComponent::fillColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    if (mode == 0) {
        strip.fill(Adafruit_NeoPixel::Color(r, g, b));
        strip.show();
    }
}

void LedStripComponent::setMode(const uint8_t mode, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t arg) {
    this->mode = mode;
    modeInfo.r = r;
    modeInfo.g = g;
    modeInfo.b = b;
    modeInfo.arg = arg;
    modeState = 0;

    update();
}

void LedStripComponent::update() {
    if (millis() - lastUpdate >= CYCLE_INTERVAL) {
        if (mode != 0) {
            switch (mode) {
                case 1:
                    if (modeInfo.arg == 0) {
                        mode = 0;
                        return;
                    }
                    if (!modeState) {
                        strip.fill(Adafruit_NeoPixel::Color(modeInfo.r, modeInfo.g, modeInfo.b));
                    } else {
                        strip.fill(Adafruit_NeoPixel::Color(0, 0, 0));
                        modeInfo.arg--;
                    }
                    modeState = !modeState;
                    strip.show();
                default:
                    break;
            }
        }
        lastUpdate = millis();
    }
}
