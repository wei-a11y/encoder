// #include <Arduino.h>
// #include "Esp32PcntEncoder.h"
// #include <Pidcontroller.h>


// Esp32PcntEncoder encoders[2];
// Pidcontroller pid_controller;

// // constexpr uint8_t MOTOR_PIN_PWM = 12;
// // constexpr uint8_t MOTOR_PIN_in1 = 14;
// // constexpr uint8_t MOTOR_PIN_in2 = 27;

// constexpr uint8_t MOTOR_PIN_PWM = 33;
// constexpr uint8_t MOTOR_PIN_in1 = 26;
// constexpr uint8_t MOTOR_PIN_in2 = 25;


// constexpr uint8_t STBY = 13; // 左轮电机ID为0，右轮电机ID为1
// constexpr uint8_t PWM_CHANNEL = 0;
// constexpr uint32_t PWM_FREQ = 20000;
// constexpr uint8_t PWM_RES = 8;
// constexpr uint32_t CONTROL_PERIOD_MS = 20;
// constexpr float TICK_DISTANCE_MM = 0.00503f;
// constexpr float SPEED_FILTER_ALPHA = 0.25f;

// float target_linear_speed = 100.0f; // 目标线速度 mm/s
// float target_angular_speed = 0.0f; // 目标角速度 rad/s
// float out_left_speed ;
// float out_right_speed ;
// int64_t last_ticks = 0;
// int64_t delta_ticks = 0;
// int64_t last_time = 0;
// float current_speed[2] = {0.0f, 0.0f};
// float filtered_speed = 0.0f;

// // float target_speed = 200.0f;
// unsigned char i = 190;  // 初始速度值
// void pwm_init() {
//     ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
//     ledcAttachPin(MOTOR_PIN_PWM, PWM_CHANNEL);
// }
// void pwm_set_speed(int16_t pwm_percent) {
//     pwm_percent = constrain(pwm_percent, -100, 100);
//     uint8_t duty = static_cast<uint8_t>(abs(pwm_percent) * 255 / 100);

//     if (pwm_percent >= 0) {
//         digitalWrite(MOTOR_PIN_in1, HIGH);
//         digitalWrite(MOTOR_PIN_in2, LOW);
//     } else {
//         digitalWrite(MOTOR_PIN_in1, LOW);
//         digitalWrite(MOTOR_PIN_in2, HIGH);
//     }

//     digitalWrite(STBY, HIGH);
//     ledcWrite(PWM_CHANNEL, duty);
// }
// void setup() {
//     Serial.begin(115200);
//     // 初始化编码器
//     //左轮编码器
//     encoders[0].init(0, 32, 35); 
//     pinMode(MOTOR_PIN_PWM, OUTPUT);
//     pinMode(MOTOR_PIN_in1, OUTPUT);
//     pinMode(MOTOR_PIN_in2, OUTPUT);
//     pinMode(STBY, OUTPUT);
//     // 初始化电机
//     digitalWrite(MOTOR_PIN_in1, HIGH);
//     digitalWrite(MOTOR_PIN_in2, LOW);
//     digitalWrite(STBY, LOW); // 使能电机
//     pwm_init();
    
//     pid_controller.update_pid(0.2, 0.015, 0.01);
//     pid_controller.out_limit(-100, 100);
//     pid_controller.update_target(target_linear_speed);

//     last_time = millis();
//     last_ticks = encoders[0].getTicks();
// }
// void loop() {
//     uint32_t now = millis();
//     uint32_t dt = now - last_time;
//     if (dt < CONTROL_PERIOD_MS) {
//         return;
//     }

//     int32_t ticks = encoders[0].getTicks();
//     delta_ticks = ticks - last_ticks;

//     float raw_speed = static_cast<float>(delta_ticks) * TICK_DISTANCE_MM * 1000.0f / static_cast<float>(dt);
//     filtered_speed += SPEED_FILTER_ALPHA * (raw_speed - filtered_speed);
//     current_speed[0] = filtered_speed;

//     last_time = now;
//     last_ticks = ticks;

//     int16_t pwm_output = static_cast<int16_t>(pid_controller.update(current_speed[0]));
//     pwm_set_speed(pwm_output);

//     Serial.printf("Speed: %.3f mm/s---Raw: %.3f---Left Encoder Ticks: %ld---pid: %d\n",
//                   current_speed[0],
//                   raw_speed,
//                   static_cast<long>(last_ticks),
//                   pwm_output);
// }