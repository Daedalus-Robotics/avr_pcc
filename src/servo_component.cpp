#include "servo_component.hpp"

#include "comm.hpp"
#include "topics.h"
#include "../../../.platformio/packages/toolchain-gccarmnoneeabi/arm-none-eabi/include/c++/9.3.1/chrono"

#define STATUS_REFRESH_INTERVAL 100
#define REFRESH_INTERVAL 5000
#define OSCILLATOR_FREQ 27000000
#define PWM_FREQ 50

ServoComponent::ServoComponent() : connected(false), lastUpdate(0), enabled(false), microsecondsArr() {}

void ServoComponent::setup() {
    if (checkConnection()) {
        sendConfig();
        driver.sleep();
        connected = true;
    } else {
        sendError(COMPONENT_SERVOS);
    }

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

void ServoComponent::status() const {
    if (!connected) {
        sendError(COMPONENT_SERVOS);
    }
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
    const uint32_t time_passed = millis() - lastUpdate;

    if (time_passed >= REFRESH_INTERVAL) {
        if (connected) {
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
            sendError(COMPONENT_SERVOS);
        }

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
    Wire.beginTransmission(PCA9685_I2C_ADDRESS);
    return Wire.endTransmission() == 0;
}

void ServoComponent::sendConfig() {
    driver.begin();
    driver.setOscillatorFrequency(OSCILLATOR_FREQ);
    driver.setPWMFreq(PWM_FREQ);
}
