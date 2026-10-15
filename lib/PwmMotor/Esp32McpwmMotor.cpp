#include "Esp32McpwmMotor.h"

/** *绑定电机引脚和LEDC通道
* pwm_io: 电机使能引脚即L298N的ENA或ENB引脚
* id: 电机ID（0或1）区别左右轮
* gpioIn1: 电机输入1引脚
* gpioIn2: 电机输入2引脚
* 该函数会初始化LEDC通道并绑定到gpioIn1引脚，同时
* 设置gpioIn2为输出引脚并初始化为低电平，最后设置pwm_io为输出引脚并初始化为低电平
*/
void Esp32McpwmMotor::attachMotor(uint8_t pwm_io, uint8_t id, uint8_t gpioIn1, uint8_t gpioIn2, uint8_t stby_io)
{
    this->pwm_io = pwm_io;
    this->gpioIn1 = gpioIn1;
    this->gpioIn2 = gpioIn2;
    this->stby_io = stby_io;
    ledc_channel = id + 8;
    // 初始化LEDC通道
    ledcSetup(ledc_channel, ledc_freq, ledc_res);
    // 绑定LEDC通道
    ledcAttachPin(pwm_io, ledc_channel);
    
    pinMode(gpioIn1, OUTPUT);
    digitalWrite(gpioIn1, LOW);
    pinMode(gpioIn2, OUTPUT);
    digitalWrite(gpioIn2, LOW);
    // pinMode(pwm_io, OUTPUT);
    // digitalWrite(pwm_io, LOW);
}

// 电机执行指令，传入轮子ID和PWM值
void Esp32McpwmMotor::updateMotorSpeed(int8_t wheel_id, int16_t pwmValue)
{
    // int speed = abs(pwmValue) * 255 / 100;
    uint8_t duty = static_cast<uint8_t>(abs(pwmValue) * 255 / 100);
    ledcWrite(ledc_channel, duty);
    if (pwmValue >= 0) {
        digitalWrite(gpioIn1, HIGH);
        digitalWrite(gpioIn2, LOW);
    } else {
        digitalWrite(gpioIn1, LOW);
        digitalWrite(gpioIn2, HIGH);
    }
}
// 停止电机
void Esp32McpwmMotor::stopMotor(int8_t motorId)
{
    digitalWrite(stby_io, LOW);
    digitalWrite(gpioIn1, LOW);
    digitalWrite(gpioIn2, LOW);
    ledcWrite(ledc_channel, 0);
}
/*
// oled接口
String motor_state = "";
constexpr uint8_t SDA_PIN = 23;
constexpr uint8_t SCL_PIN = 18;
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL_PIN, SDA_PIN, U8X8_PIN_NONE);
*/

/*
void oled_init()
{
    // 初始化 OLED
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(10, 30, "Motor Control");
    u8g2.sendBuffer();
    delay(1000);
}
//oled屏幕输出
void oled_display(int speed, String command)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0,20);
    u8g2.print("Speed:");
    u8g2.print(speed);
    u8g2.setCursor(0,40);
    u8g2.print("State:");
    u8g2.print(command);
    u8g2.sendBuffer();
}
*/
