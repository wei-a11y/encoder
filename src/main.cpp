#include <Arduino.h>
#include "Esp32PcntEncoder.h"
#include <Pidcontroller.h>


Esp32PcntEncoder encoders[2];
Pidcontroller pid_controller;

constexpr uint8_t MOTOR_PIN_PWM = 14;
constexpr uint8_t MOTOR_PIN_in1 = 27;
constexpr uint8_t MOTOR_PIN_in2 = 26;

constexpr uint8_t STBY = 13; // 左轮电机ID为0，右轮电机ID为1
constexpr uint8_t PWM_CHANNEL = 0;
constexpr uint32_t PWM_FREQ = 20000;
constexpr uint8_t PWM_RES = 8;

float target_linear_speed = 200.0f; // 目标线速度 mm/s
float target_angular_speed = 0.0f; // 目标角速度 rad/s
float out_left_speed ;
float out_right_speed ;
int64_t last_ticks = 0;
int64_t delta_ticks;
int64_t last_time = 0;
float current_speed[2];


unsigned char i = 128;  // 初始速度值
void pwm_init() {
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
    ledcAttachPin(MOTOR_PIN_PWM, PWM_CHANNEL);
}
void pwm_set_speed(uint8_t speed) {
    // 反向映射:speed 越大,占空比越大,电压越高
    // 如果想要反向(speed 越小电压越高),改为: ledcWrite(PWM_CHANNEL, 255 - speed);
    digitalWrite(STBY, HIGH); // 使能电机
    ledcWrite(PWM_CHANNEL, speed);
}
void setup() {
    Serial.begin(115200);
    // 初始化编码器
    //左轮编码器
    encoders[0].init(0, 23, 22); 
    pinMode(MOTOR_PIN_PWM, OUTPUT);
    pinMode(MOTOR_PIN_in1, OUTPUT);
    pinMode(MOTOR_PIN_in2, OUTPUT);
    pinMode(STBY, OUTPUT);
    // 初始化电机
    digitalWrite(MOTOR_PIN_in1, HIGH);
    digitalWrite(MOTOR_PIN_in2, LOW);
    digitalWrite(STBY, LOW); // 使能电机
    pwm_init();
    
    pid_controller.update_pid(0.09, 0.06, 0.01);
    pid_controller.out_limit(-100, 100);
    pid_controller.update_target(target_linear_speed);
}
void loop() {
    // last_ticks = encoders[0].getTicks();
    Serial.printf("Left Encoder Ticks: %lld\n", last_ticks);
    // 获取编码器计数
    uint64_t dt = millis() - last_time;
    if(dt == 0)
    return;
	delta_ticks = encoders[0].getTicks() - last_ticks;
    last_ticks = encoders[0].getTicks();
	current_speed[0] = float(delta_ticks * 0.08785f) / dt * 1000; // mm/s
	last_time = millis();

    pwm_set_speed(pid_controller.update(current_speed[0]));
    // pwm_set_speed(200);
    delay(50);
}