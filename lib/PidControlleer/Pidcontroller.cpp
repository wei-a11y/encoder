#include "Pidcontroller.h"
#include "Arduino.h"
void Pidcontroller::intergral_reset(float current)
{
    float abs_current = fabs(current);
    if(abs_current <= 100)
    {
        this->intergral_up_ = 100;
        return;
    }
    else if(abs_current <= 210)
    {
        this->intergral_up_ = 50;
        return;
    }
    else if(abs_current <= 400)
    {
        this->intergral_up_ = 50;
        return;
    }
    else if(abs_current <= 590)
    {
        this->intergral_up_ = 150;
        return;
    }
    else if(abs_current <= 700)
    {
        this->intergral_up_ = 200;
        return;
    }
}
Pidcontroller::Pidcontroller(float kp, float ki, float kd)
{
    reset();
    update_pid(kp, ki, kd);
}      
float Pidcontroller::update(float current)
{
    // intergral_reset(current);
    float error = target_ - current;//计算误差
    derror_ = error - error_last_;//计算误差变化率
    error_last_ = error;//保存上次误差


    error_sum_ += error;//累积误差
    //积分限幅
    if (error_sum_ > intergral_up_)
        error_sum_ = intergral_up_;
    else if (error_sum_ < -1 * intergral_up_)
        error_sum_ = -1 * intergral_up_;
    float output = kp_ * error + ki_ * error_sum_ + kd_ * derror_;//计算PID输出
    //输出限幅
    if (output > out_max_)
        output = out_max_;
    else if (output < out_min_)
        output = out_min_;

    return output;
}

void Pidcontroller::update_target(float target)
{
    target_ = target;//更新目标值
}

void Pidcontroller::update_pid(float kp, float ki, float kd)
{
    // 只重置误差累积，不重置输出限幅
    error_sum_ = 0.0f;
    derror_ = 0.0f;
    error_last_ = 0.0f;
    error_pre_ = 0.0f;
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}
void Pidcontroller::reset()
{
    //重置PID控制器状态
    target_ = 0.0f;
    out_min_ = 0.0f;
    out_max_ = 0.0f;
    kp_ = 0.0f;
    ki_ = 0.0f;
    kd_ = 0.0f;
    error_sum_ = 0.0f;
    derror_ = 0.0f;
    error_last_ = 0.0f;
    error_pre_ = 0.0f;
}
void Pidcontroller::out_limit(float out_min, float out_max)
{
    out_min_ = out_min;
    out_max_ = out_max;
}