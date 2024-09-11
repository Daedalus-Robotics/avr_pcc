#pragma once

#include <cstdint>
#include <functional>

#define MESSAGE_BUF_LEN (static_cast<int>(sizeof(message_t)))
#define SOL_CHAR 0b1111111111111111

#pragma pack(1)
struct message_t {
    uint16_t sol;
    uint16_t identifier; // The id or topic of the message
    uint8_t data[8]; // The data payload of the message (64 bits)
    uint16_t crc; // The CRC 15 checksum of the data for error checking with an extra bit because why not
};
#pragma pack()

typedef std::function<void(const message_t *)> topic_callback;

// Calculates the CRC checksum and compares it with the one in the message.
bool checkCrc(const message_t *message);
// Register a callback to be called whenever a specific identifier is received
void registerTopic(uint16_t identifier, const topic_callback& callback);
// Send a message (will populate crc automatically)
void sendMessage(message_t *message);
// Call any callbacks registered for a specific message.
bool dispatchCallback(const message_t *message);
// Send an error frame
void sendError(uint8_t code, uint8_t data = 0);
// Send a status update
void sendStatus(uint8_t code);
