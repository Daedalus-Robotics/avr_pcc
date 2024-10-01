#include <Adafruit_SleepyDog.h>
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <vector>

#include "comm.hpp"
#include "led_strip.hpp"
#include "onboard_led.hpp"
#include "servo_component.hpp"
#include "thermal_component.hpp"
#include "topics.h"

bool hasNotifiedReady = false;

auto onboardLedComponent = OnboardLedComponent();
auto ledStripComponent = LedStripComponent(30);
auto servoComponent = ServoComponent();
auto thermalComponent = ThermalComponent();

std::vector<uint8_t> dataQueue;

[[noreturn]] void reset() {
    Watchdog.disable();

    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(100);

    Watchdog.enable(1);
    while (true) {}
}

[[noreturn]] void halt() {
    Watchdog.disable();

    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    digitalWrite(13, HIGH);

    while (true) {}
}

extern "C" void setup() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    onboardLedComponent.setColor(0, 255, 50);

    Serial.begin(115200);

    registerTopic(TOPIC_RESET, [](const message_t *) {
        sendStatus(STATUS_RESET);

        onboardLedComponent.reset();
        ledStripComponent.reset();
        servoComponent.reset();
        thermalComponent.reset();

        reset();
    });
    registerTopic(TOPIC_ESTOP, [](const message_t *) {
        sendStatus(STATUS_HALT);

        onboardLedComponent.eStop();
        ledStripComponent.eStop();
        servoComponent.eStop();
        thermalComponent.eStop();

        halt();
    });

    onboardLedComponent.setup();
    ledStripComponent.setup();
    servoComponent.setup();
    thermalComponent.setup();

    onboardLedComponent.setColor(0, 0, 0);
    digitalWrite(13, LOW);

    Watchdog.enable(2500);
}

extern "C" void loop() {
    static uint8_t currentByte;

    if (!Serial) {
        hasNotifiedReady = false;
    } else if (!hasNotifiedReady) {
        delay(50);
        sendStatus(STATUS_READY);
        hasNotifiedReady = true;
    }

    if (Serial.available() > 0) {
        if (dataQueue.size() >= MESSAGE_BUF_LEN) {
            dataQueue.erase(dataQueue.cbegin());
        }
        Serial.read(&currentByte, 1);
        dataQueue.push_back(currentByte);

        if (dataQueue.size() == MESSAGE_BUF_LEN && dataQueue[0] == 255 && dataQueue[1] == 255) {
            const auto message = reinterpret_cast<message_t *>(dataQueue.data());

            if (true) {// !checkCrc(message)) {
                dataQueue.clear();
                if (!dispatchCallback(message)) {
                    sendError(ERROR_BAD_TOPIC, message->identifier);
                }
            }
        }
    }

    // servoComponent.update();
    thermalComponent.update();

    Watchdog.reset();
}
