#ifndef __ESP32MCPWMMOTOR_H__
#define __ESP32MCPWMMOTOR_H__

#include <Arduino.h>

class Esp32McpwmMotor
{
private:
    uint16_t ledc_freq = 2000;
    uint8_t ledc_res = 8;
    uint8_t ledc_channel;
    uint8_t gpioIn1;
    uint8_t gpioIn2;
    uint8_t en_io;

public:
    Esp32McpwmMotor() = default;
    ~Esp32McpwmMotor() = default;
    void attachMotor(uint8_t en_io, uint8_t id, uint8_t gpioIn1, uint8_t gpioIn2);
    void stopMotor(int8_t motorId = -1);
    void updateMotorSpeed(int8_t wheel_id, int16_t pwmValue);
};

#endif