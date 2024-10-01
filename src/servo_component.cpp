#include "servo_component.hpp"

#include "comm.hpp"
#include "topics.h"
#include "../../../.platformio/packages/toolchain-gccarmnoneeabi/arm-none-eabi/include/c++/9.3.1/chrono"

#define REFRESH_INTERVAL 15000
#define OSCILLATOR_FREQ 27000000
#define PWM_FREQ 50

ServoComponent::ServoComponent() : lastConnected(false), lastUpdate(0), enabled(false), microsecondsArr() {}

void ServoComponent::setup() {
    sendConfig();
    driver.sleep();

    registerTopic(TOPIC_SERVO_ENABLE, [this](const message_t *message) {
        setEnabled(message->data[0] != 0);
    });
    registerTopic(TOPIC_SERVO_SET, [this](const message_t *message) {
        const auto microsecondsPtr = reinterpret_cast<const uint16_t *>(&message->data[1]);
        setMicroseconds(message->data[0], *microsecondsPtr);
    });
    registerTopic(TOPIC_SERVO_SPAN, [this](const message_t *message) {
        const auto microsecondsArrPtr = reinterpret_cast<const uint16_t *>(&message->data[2]);
        for (uint16_t i = 0; i < message->data[1]; i++) {
            setMicroseconds(i + message->data[0], microsecondsArrPtr[i]);
        }
    });
}

void ServoComponent::reset() {
    driver.wakeup();
    delay(1);
    for (uint8_t i = 0; i < 8; i++) {
        driver.writeMicroseconds(i, microsecondsArr[i]);
    }
    driver.reset();
}

void ServoComponent::eStop() {
    driver.wakeup();
    delay(1);
    for (uint8_t i = 0; i < 8; i++) {
        driver.writeMicroseconds(i, microsecondsArr[i]);
    }
    driver.sleep();
}

void ServoComponent::update() {
    if ((checkConnection() && !lastConnected) || millis() - lastUpdate >= REFRESH_INTERVAL) {
        lastConnected = true;
        lastUpdate = millis();

        setEnabled(enabled);
        delay(1);
        if (enabled) {
            sendConfig();
            delay(1);
        }
        for (uint8_t i = 0; i < 8; i++) {
            driver.writeMicroseconds(i, microsecondsArr[i]);
        }

        delay(1);
    } else {
        lastConnected = false;
    }
}

void ServoComponent::setEnabled(const bool enabled) {
    this->enabled = enabled;
    if (enabled) {
        driver.wakeup();
        sendConfig();
    } else {
        driver.sleep();
    }
}

void ServoComponent::setMicroseconds(const uint8_t num, const uint16_t microseconds) {
    this->microsecondsArr[num] = microseconds;
    driver.writeMicroseconds(num, microseconds);
}

bool ServoComponent::checkConnection() {
    const int res = driver.readPrescale();
    return res != 0;
}

void ServoComponent::sendConfig() {
    driver.begin();
    driver.setOscillatorFrequency(OSCILLATOR_FREQ);
    driver.setPWMFreq(PWM_FREQ);
}
