#include "onboard_led.hpp"

#include "comm.hpp"
#include "topics.h"

OnboardLedComponent::OnboardLedComponent() : onboard(1, PIN_NEOPIXEL, NEO_GRB) {
    onboard.begin();
    onboard.setPixelColor(0, 0, 0, 0);
    onboard.setBrightness(255);
    onboard.show();
}

void OnboardLedComponent::setup() {
    registerTopic(TOPIC_ONBOARD_LED_SET, [this](const message_t *message) -> void {
        setColor(message->data[0], message->data[1], message->data[2]);
    });
}

void OnboardLedComponent::reset() {
    setColor(255, 0, 100);
}

void OnboardLedComponent::eStop() {
    setColor(255, 35, 0);
}

void OnboardLedComponent::setColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    onboard.setPixelColor(0, r, g, b);
    onboard.show();
}
