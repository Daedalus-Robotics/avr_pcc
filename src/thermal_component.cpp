#include "thermal_component.hpp"

#include "comm.hpp"
#include "topics.h"

#define REFRESH_RATE 10
#define REFRESH_INTERVAL (60 / REFRESH_RATE)

ThermalComponent::ThermalComponent() : isConnected(false), lastUpdate(millis()) {}

void ThermalComponent::setup(){
    if (amg.begin()) {
        isConnected = true;
    } else {
        sendError(ERROR_COMPONENT_FAILED, COMPONENT_THERMAL);
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ThermalComponent::reset() {}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ThermalComponent::eStop() {}

void ThermalComponent::update() {
    if (isConnected && millis() - lastUpdate >= REFRESH_INTERVAL) {
        static float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
        amg.readPixels(pixels);

        size_t lineNum = 0;
        static message_t message = {
            SOL_NUM
        };

        message.identifier = TOPIC_THERMAL_ROW_0;
        memcpy(message.data, &pixels[lineNum++ * 8], AMG88xx_PIXEL_ARRAY_SIZE/8);
        sendMessage(&message);

        message.identifier = TOPIC_THERMAL_ROW_1;
        memcpy(message.data, &pixels[lineNum++ * 8], AMG88xx_PIXEL_ARRAY_SIZE / 8);
        sendMessage(&message);

        message.identifier = TOPIC_THERMAL_ROW_2;
        memcpy(message.data, &pixels[lineNum++ * 8], AMG88xx_PIXEL_ARRAY_SIZE / 8);
        sendMessage(&message);

        message.identifier = TOPIC_THERMAL_ROW_3;
        memcpy(message.data, &pixels[lineNum++ * 8], AMG88xx_PIXEL_ARRAY_SIZE / 8);
        sendMessage(&message);

        message.identifier = TOPIC_THERMAL_ROW_4;
        memcpy(message.data, &pixels[lineNum++ * 8], AMG88xx_PIXEL_ARRAY_SIZE / 8);
        sendMessage(&message);

        message.identifier = TOPIC_THERMAL_ROW_5;
        memcpy(message.data, &pixels[lineNum++ * 8], AMG88xx_PIXEL_ARRAY_SIZE / 8);
        sendMessage(&message);

        message.identifier = TOPIC_THERMAL_ROW_6;
        memcpy(message.data, &pixels[lineNum++ * 8], AMG88xx_PIXEL_ARRAY_SIZE / 8);
        sendMessage(&message);

        message.identifier = TOPIC_THERMAL_ROW_7;
        memcpy(message.data, &pixels[lineNum * 8], AMG88xx_PIXEL_ARRAY_SIZE / 8);
        sendMessage(&message);
        lastUpdate = millis();
    }
}
