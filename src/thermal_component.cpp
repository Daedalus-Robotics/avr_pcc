#include "thermal_component.hpp"

#include "comm.hpp"
#include "topics.h"

#define REFRESH_RATE 10
#define REFRESH_INTERVAL (60 / REFRESH_RATE)

ThermalComponent::ThermalComponent() : isConnected(false), lastUpdate(millis()), rowCount() {}

void ThermalComponent::setup(){
    if (amg.begin()) {
        isConnected = true;
    } else {
        sendError(COMPONENT_THERMAL);
    }
}

void ThermalComponent::status() const {
    if (!isConnected) {
        sendError(COMPONENT_THERMAL);
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ThermalComponent::reset() {}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ThermalComponent::eStop() {}

void ThermalComponent::update() {
    if (isConnected && millis() - lastUpdate >= REFRESH_INTERVAL) {
        float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
        amg.readPixels(pixels);

        message_t message = {
            SOL_NUM
        };

        uint16_t topic;
        switch (rowCount) {
            case 0:
                topic = TOPIC_THERMAL_ROW_0;
                break;
            case 1:
                topic = TOPIC_THERMAL_ROW_1;
                break;
            case 2:
                topic = TOPIC_THERMAL_ROW_2;
                break;
            case 3:
                topic = TOPIC_THERMAL_ROW_3;
                break;
            case 4:
                topic = TOPIC_THERMAL_ROW_4;
                break;
            case 5:
                topic = TOPIC_THERMAL_ROW_5;
                break;
            case 6:
                topic = TOPIC_THERMAL_ROW_6;
                break;
            case 7:
                topic = TOPIC_THERMAL_ROW_7;
                break;
            default:
                rowCount = 0;
                return;
        }

        message.identifier = topic;
        auto *dataPtr = reinterpret_cast<float *>(message.data);
        for (size_t i = 0; i < 8; i++) {
            dataPtr[i] = pixels[(rowCount * 8) + i];
        }
        sendMessage(&message);

        rowCount = (rowCount + 1) % 8;
        lastUpdate = millis();
    }
}
