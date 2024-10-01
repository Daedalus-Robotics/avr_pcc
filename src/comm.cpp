#include "comm.hpp"

#include <Arduino.h>
#include <unordered_map>

#include "crc15can.hpp"
#include "topics.h"

static std::unordered_map<uint16_t, topic_callback> topic_callbacks;

inline uint16_t get_crc(const message_t *message) {
    return crc15can_byte(0, message->data, MESSAGE_BUF_LEN);
}

inline void append_crc(message_t *message) {
    message->crc = get_crc(message);
}

bool checkCrc(const message_t *message) {
    return message->crc == get_crc(message);
}

void registerTopic(uint16_t identifier, const topic_callback& callback) {
    topic_callbacks.insert(std::pair<uint16_t, topic_callback>(identifier, callback));
}

void sendMessage(message_t *message) {
    append_crc(message);
    if (!Serial.availableForWrite()) {
        Serial.flush(); // The receiving end will request again if the data was necessary
    }
    Serial.write(reinterpret_cast<const uint8_t *>(message), MESSAGE_BUF_LEN);
}

bool dispatchCallback(const message_t *message) {
    const bool foundTopic = topic_callbacks.find(message->identifier) != topic_callbacks.end();
    if (foundTopic) {
        topic_callbacks[message->identifier](message);
    }
    return foundTopic;
}

void sendError(const uint8_t code, const uint8_t data) {
    message_t errorFrame = {
        SOL_NUM,
        TOPIC_ERROR
    };
    errorFrame.data[0] = code;
    errorFrame.data[1] = data;
    sendMessage(&errorFrame);
}

void sendStatus(const uint8_t code) {
    message_t statusFrame = {
        SOL_NUM,
        TOPIC_STATUS
    };
    statusFrame.data[0] = code;
    sendMessage(&statusFrame);
}
