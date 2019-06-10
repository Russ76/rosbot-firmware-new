#ifndef __ROSBOT_DRIVE_H__
#define __ROSBOT_DRIVE_H__

#include <mbed.h>
#include <arm_math.h>
#include "drv88xx-driver-mbed/DRV8848.h"
#include "encoder-mbed/Encoder.h"
#define PWM_DEFAULT_FREQ_HZ 18000UL

enum RosbotMotNum
{
    FR = 0,
    FL = 1,
    RR = 2,
    RL = 3
};

enum SpeedMode
{
    TICSKPS,
    RPM,
    RPM_NOGEAR,
    MPS,
    DUTY_CYCLE,
};

enum RosbotDriveStates
{
    UNINIT,
    HALT,
    IDLE,
    OPERATIONAL,
    FAULT
};

typedef struct RosobtDrivePid
{
    float kp;
    float ki;
    float kd;
    float out_min;
    float out_max;
    uint32_t dt_ms;
}RosbotDrivePid_t;

typedef struct RosbotWheel
{
    float radius;
    float diameter_modificator;
    float tyre_deflection;
    float gear_ratio;
    uint32_t encoder_cpr; // conuts per revolution
}RosbotWheel_t;

typedef struct RosbotDriveParams
{
    RosbotDrivePid_t pid_params;
    RosbotWheel_t wheel_params;
    uint8_t polarity; // LSB -> motor, MSB -> enkoder
}RosbotDrive_params_t;

typedef struct NewTargetSpeed
{
    float speed[4];
    SpeedMode mode;
    NewTargetSpeed()
    :speed{0,0,0,0},mode(MPS)
    {}
}NewTargetSpeed_t;

typedef struct PidDebugData
{
    float cspeed;
    float tspeed;
    float pidout;
    float error;
}PidDebugData_t;

class RosbotDrive
{
public:
    static const RosbotWheel_t DEFAULT_WHEEL_PARAMS;
    static const RosbotDrivePid_t DEFAULT_PID_PARAMS;
    static RosbotDrive * getInstance(const RosbotDrive_params_t * params);
    static int getRosbotDriveType();
    void init(int freq=PWM_DEFAULT_FREQ_HZ);
    void enable(bool en=true);
    void stop();
    void enablePidReg(bool en);
    bool isPidEnabled();
    float getSpeed(RosbotMotNum mot_num);
    // float getSpeed(RosbotMotNum mot_num, SpeedMode mode);
    float getDistance(RosbotMotNum mot_num);
    void resetDistance();
    int32_t getEncoderTicks(RosbotMotNum mot_num);
    void updateTargetSpeed(const NewTargetSpeed_t * new_speed);
    void updateWheelParams(const RosbotWheel_t * params);
    void updatePidParams(const RosbotDrivePid_t * params, bool reset);
    void getPidDebugData(PidDebugData_t * data, RosbotMotNum mot_num);
    
private:
    static RosbotDrive * _instance;
    void regulatorLoop();
    RosbotDrive(const RosbotDrive_params_t * params);
    volatile RosbotDriveStates _state;
    volatile bool _pid_state;
    RosbotDrivePid_t _pid_params;
    uint8_t _polarity;
    volatile double _tspeed_mps[4];
    volatile double _cspeed_mps[4];
    volatile int32_t _cdistance[4];
    volatile double _error[4];
    volatile float _pidout[4];
    double _wheel_coefficient;
    DRV8848 * _mot_driver[2];
    DCMotor * _mot[4]; 
    Encoder * _encoder[4];
    arm_pid_instance_f32 * _pid_instance[4];
    Mutex rosbot_drive_mutex;
};

#endif /* __ROSBOT_DRIVE_H__ */