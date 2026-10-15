#ifndef __KINEMATICS_H__
#define __KINEMATICS_H__

#include <Arduino.h>
typedef struct
{
    float per_pulse_distance;      /* 单个脉冲对应的轮子前进距离 */
    float motor_speed = 0.0f;      /* 当前电动机速度 mm/s, 计算时使用 */
    int64_t last_encoder_trick = 0;/* 上次电动机的编码器读数 */
    float alpha = 0.25;
} motor_parm_t;
typedef struct
{
    float x;            /* x方向位移 */
    float y;            /* y方向位移 */
    float angle;        /* 角度 */
    float linear_speed; /* 线速度 */
    float angle_speed;  /* 角速度 */
}odom_t;

class Kinematics
{
public:
    /* 构造函数，默认实现 */
    Kinematics() = default;
    /* 析构函数，默认实现 */
    ~Kinematics() = default;
    // 设置电机参数
    void set_motor_params(uint8_t id, float per_pulse_distance);
    // 设置两轮间距
    void set_wheel_distance(float wheel_distance);
    // 逆运动学计算
    void kinematics_inverse(float linear_speed, float angle_speed, float &out_left_speed, float &out_right_speed);
    // 正运动学计算
    void kinematics_forward(float left_speed, float right_speed, float &out_linear_speed, float &out_angle_speed);
    // 更新电机速度
    void update_motor_speed(uint64_t current_time, int64_t left_trick, int64_t right_trick);
    // 获取电机速度
    float get_motor_speed(uint8_t id);
    /* 更新里程计数据 */
    void update_odom(uint16_t dt);
    /* 获取里程计数据 */
    odom_t &get_odom();
    /* 将角度转换到-pi到 pi的范围内 */
    static void TransAngleInPI(float angle, float &out_angle);

private:
    odom_t odom_ = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};  // 初始化为 0
    motor_parm_t motor_param_[2];      /* 两个电机参数 */
    uint64_t last_update_time_ = 0;   /* 上次计算时间 */
    float wheel_distance_;           /* 两轮间距 */
};


#endif