#include "main.h"

Controller controller (CONTROLLER_MASTER);

Motor drive1 (1, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive2 (2, MOTOR_GEAR_600, true , MOTOR_ENCODER_DEGREES);
Motor drive3 (3, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1,drive2,drive3});

Motor drive4 (4, MOTOR_GEAR_600, true , MOTOR_ENCODER_DEGREES);
Motor drive5 (5, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive6 (6, MOTOR_GEAR_600, true , MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4,drive5,drive6});

Motor_Group drivetrain ({drive1,drive2,drive3, drive4,drive5,drive6});

Rotation xTracking (10);
Rotation yTracking (11);

IMU inertial1 (12);
IMU inertial2 (13);
IMU inertial3 (14);