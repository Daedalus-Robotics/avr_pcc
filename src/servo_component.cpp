#include "servo_component.hpp"

#include "comm.hpp"
#include "topics.h"

typedef union {
    uint8_t msgData[2];
    uint16_t microseconds;
} servo_microseconds;

ServoComponent::ServoComponent() = default;

void ServoComponent::setup() {
    driver.begin();
    driver.setOscillatorFrequency(27000000);
    driver.setPWMFreq(50);
    driver.sleep();

    registerTopic(TOPIC_SERVO_ENABLE, [this](const message_t *message) {
        setEnabled(message->data[0] != 0);
    });
    registerTopic(TOPIC_SERVO_SET, [this](const message_t *message) {
        const auto uSeconds = reinterpret_cast<const servo_microseconds *>(&message->data[1]);
        setMicroseconds(message->data[0], uSeconds->microseconds);
    });
}

void ServoComponent::reset() {
    driver.sleep();
    driver.reset();
}

void ServoComponent::eStop() {
    driver.sleep();
}

void ServoComponent::setEnabled(const bool enabled) {
    if (enabled) {
        driver.wakeup();
    } else {
        driver.sleep();
    }
}

void ServoComponent::setMicroseconds(const uint8_t num, const uint16_t microseconds) {
    driver.writeMicroseconds(num, microseconds);
}
