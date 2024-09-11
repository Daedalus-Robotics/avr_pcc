#pragma once

// Request topic (host-mcu)
#define TOPIC_RESET 0
#define TOPIC_ESTOP 1

#define TOPIC_ONBOARD_LED_SET 2

#define TOPIC_LED_STRIP_SET 3
#define TOPIC_LED_STRIP_FILL 4
#define TOPIC_LED_STRIP_MODE 5

#define TOPIC_SERVO_ENABLE 6
#define TOPIC_SERVO_SET 7

// Response topic (mcu-host)
#define TOPIC_ERROR 0
#define TOPIC_STATUS 0

// Error Codes
#define ERROR_BAD_TOPIC 0

// Status Codes
#define STATUS_READY 0
#define STATUS_RESET 1
#define STATUS_HALT 2
