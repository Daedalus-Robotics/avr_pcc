#include "led_strip.hpp"

#include "comm.hpp"
#include "topics.h"

LedStripComponent::LedStripComponent(uint16_t pixelCount) : strip(pixelCount, 5, NEO_GRB) {
    pinMode(10, OUTPUT);
}

void LedStripComponent::setup() {
    digitalWrite(10, HIGH);

    strip.begin();
    strip.fill(0, 0, 0);
    strip.setBrightness(255);
    strip.show();

    registerTopic(TOPIC_LED_STRIP_SET, [this](const message_t *message) -> void {
        setColor(message->data[0], message->data[1], message->data[2], message->data[3]);
    });
    registerTopic(TOPIC_LED_STRIP_FILL, [this](const message_t *message) -> void {
        fillColor(message->data[0], message->data[1], message->data[2]);
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
    strip.setPixelColor(n, Adafruit_NeoPixel::Color(r, g, b));
    strip.show();
}

void LedStripComponent::fillColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    strip.fill(Adafruit_NeoPixel::Color(r, g, b));
    strip.show();
}
