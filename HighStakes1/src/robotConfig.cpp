#include "main.h"

Controller controller (CONTROLLER_MASTER);

Motor drive1 (16, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive2 (15, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive3 (7, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1,drive2,drive3});

Motor drive4 (20, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive5 (18, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive6 (17, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4,drive5,drive6});

Motor_Group drivetrain ({drive1,drive2,drive3, drive4,drive5,drive6});

Motor intake (4, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);

Rotation xTracking (11);
Rotation yTracking (12);

IMU inertial1 (2);
IMU inertial2 (13);
IMU inertial3 (14);