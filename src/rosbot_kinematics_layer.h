#ifndef __ROSBOT_KINEMATICS_LAYER_H__
#define __ROSBOT_KINEMATICS_LAYER_H__

#include <RosbotDrive.h>

#define ROBOT_WIDTH 0.19
#define DIAMETER_MODIFICATOR 1.0f
#define TYRE_DEFLACTION 1.0f
#define GEAR_RATIO 34.014
#define ENCODER_CPR 48
#define ROBOT_WIDTH_HALF ROBOT_WIDTH/2.0f
#define WHEEL_DIAMETER 0.063
#define WHEEL_RADIUS WHEEL_DIAMETER/2.0f
#define POLARITY 0b00110000
extern const RosbotDrive_params_t ROSBOT_PARAMS;

typedef struct RosbotOdometry
{
    float wheel_FR_ang_pos;      // radians
    float wheel_FL_ang_pos;      // radians
    float wheel_RR_ang_pos;      // radians
    float wheel_RL_ang_pos;      // radians
    float wheel_L_ang_pos;       // radians
    float wheel_R_ang_pos;       // radians
    float wheel_L_ang_vel = 0;   // radians per second
    float wheel_R_ang_vel = 0;   // radians per second
    float robot_angular_pos = 0; // radians
    float robot_angular_vel = 0; // radians per second
    float robot_x_pos = 0;       // meters
    float robot_y_pos = 0;       // meters
    float robot_x_vel = 0;       // meters per second
    float robot_y_vel = 0;       // meters per second
} RosbotOdometry_t;

void setRosbotSpeed(RosbotDrive * drive, float linear, float angular);
void updateRosbotOdometry(RosbotDrive * drive, RosbotOdometry_t * odom, float dtime);
void resetRosbotOdometry(RosbotDrive * drive, RosbotOdometry_t * odom);

#endif /* __ROSBOT_KINEMATICS_LAYER_H__ */