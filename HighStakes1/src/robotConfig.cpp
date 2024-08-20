#include "main.h"

Controller controller (CONTROLLER_MASTER);

Motor drive1 (1, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive2 (2, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive3 (3, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1,drive2,drive3});

Motor drive4 (11, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive5 (12, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive6 (14, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4,drive5,drive6});

Motor_Group drivetrain ({drive1,drive2,drive3, drive4,drive5,drive6});

Motor intake (4, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);

Rotation xTracking (5);
Rotation yTracking (6);

IMU inertial1 (7);
IMU inertial2 (8);
IMU inertial3 (9);