#include "Kinematics.h"

void Kinematics::set_motor_params(uint8_t id, float per_pulse_distance)
{
    motor_param_[id].per_pulse_distance = per_pulse_distance;//电动机每个脉冲对应的距离
}
void Kinematics::set_wheel_distance(float wheel_distance)
{
    wheel_distance_ = wheel_distance;//设置两轮间距
}
float Kinematics::get_motor_speed(uint8_t id)
{
    return motor_param_[id].motor_speed;//获取电动机速度
}
/**
 * @brief更新电动机速度和编码器读数
 * @param current_time 当前时间，单位毫秒
 * @param left_trick 左电动机编码器读数
 * @param right_trick 右电动机编码器读数
 */
void Kinematics::update_motor_speed(uint64_t current_time, int64_t left_trick, int64_t right_trick)
{
    uint32_t dt = current_time - last_update_time_;//计算时间差
    last_update_time_ = current_time;//更新上次计算时间
    int64_t dtick1 = left_trick - motor_param_[0].last_encoder_trick;//计算左电动机编码器增量
    int64_t dtick2 = right_trick - motor_param_[1].last_encoder_trick;//计算右电动机编码器增量
    motor_param_[0].last_encoder_trick = left_trick;//更新左电动机编码器读数
    motor_param_[1].last_encoder_trick = right_trick;//更新右电动机编码器读数
    //计算轮子速度
    // float raw_left_speed = float(dtick1 * motor_param_[0].per_pulse_distance) / dt * 1000;
    // float raw_right_speed = float(dtick2 * motor_param_[1].per_pulse_distance) / dt * 1000;
    // motor_param_[0].motor_speed += motor_param_[0].alpha * (raw_left_speed - motor_param_[0].motor_speed) ;
    // motor_param_[1].motor_speed += motor_param_[1].alpha * (raw_right_speed - motor_param_[1].motor_speed) ;
    motor_param_[0].motor_speed = float(dtick1 * motor_param_[0].per_pulse_distance) / dt * 1000;
    motor_param_[1].motor_speed = float(dtick2 * motor_param_[1].per_pulse_distance) / dt * 1000;
}
/**
 * @brief 正运动学计算，根据左右轮编码器增量计算线速度和角速度
 * @param left_speed 左轮速度
 * @param right_speed 右轮速度
 * @param[out] out_linear_speed 输出线速度
 * @param[out] out_angle_speed 输出角速度
 */
void Kinematics::kinematics_forward(float left_speed, float right_speed, float &out_linear_speed, float &out_angle_speed)
{
    out_linear_speed = (left_speed + right_speed) / 2.0f;//计算线速度
    out_angle_speed = (right_speed - left_speed) / wheel_distance_;//计算角速度
}
/**
 * @brief 逆运动学计算，根据线速度和角速度计算左右轮速度
 * @param linear_speed 线速度
 * @param angle_speed 角速度
 * @param[out] out_left_speed 输出左轮速度
 * @param[out] out_right_speed 输出右轮速度
 */
void Kinematics::kinematics_inverse(float linear_speed, float angle_speed, float &out_left_speed, float &out_right_speed)
{
    out_left_speed = linear_speed - (angle_speed * wheel_distance_ / 2.0f);//计算左轮速度
    out_right_speed = linear_speed + (angle_speed * wheel_distance_ / 2.0f);//计算右轮速度
}
odom_t &Kinematics::get_odom()
{
    return odom_;
}
void Kinematics::TransAngleInPI(float angle, float &out_angle)
{
    if (angle > PI)
        out_angle -= 2 * PI;
    else if (angle < -PI)
        out_angle += 2 * PI;
}
void Kinematics::update_odom(uint16_t dt)
{
    //将时间单位ms转化为s
    float dt_s = dt / 1000.0f;
    //运动学正解，获取线速度和角速度
    this->kinematics_forward(motor_param_[0].motor_speed, motor_param_[1].motor_speed, odom_.linear_speed, odom_.angle_speed);
    //转换线速度单位mm/s -> m/s
    odom_.linear_speed = odom_.linear_speed / 1000.0f;
    //计算当前角度
    odom_.angle += odom_.angle_speed * dt_s;
    //将角度转换到-pi -- pi
    Kinematics::TransAngleInPI(odom_.angle, odom_.angle);
    //由变化距离计算x、y方向的分量（累积）
    float delta_distance = odom_.linear_speed * dt_s;
    odom_.x += delta_distance * cos(odom_.angle);
    odom_.y += delta_distance * sin(odom_.angle);
}