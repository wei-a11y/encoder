#ifndef __PIDCONTROLLER_H__
#define __PIDCONTROLLER_H__

class Pidcontroller 
{
public:
    Pidcontroller() = default;
    Pidcontroller(float kp, float ki, float kd);
private:
    float target_;                 //目标值
    float out_min_ = -100;                //输出下限
    float out_max_ = 100;                //输出上限
    float kp_;                     //比例系数
    float ki_;                     //积分系数
    float kd_;                     //微分系数
    float error_sum_;              //误差累积
    float derror_;                 //误差变化率
    float error_last_;             //上次误差
    float error_pre_;              //上上次误差
    float intergral_up_ = 1500;    //积分上限
public:
    float update(float current);                    //根据当前值计算输出
    void update_target(float target);               //更新目标值
    void update_pid(float kp, float ki, float kd);  //更新PID参数
    void reset();                                   //重置PID控制器
    void out_limit(float out_min, float out_max);   //设置输出限制
    void intergral_reset(float current);             //设置积分上限

};


#endif // __PIDCONTROLLER_H__