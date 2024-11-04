#include "main.h"

Controller controller (CONTROLLER_MASTER);

Motor drive1 (13, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive2 (14, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive3 (12, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1,drive2,drive3});

Motor drive4 (11, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive5 (15, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive6 (16, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4,drive5,drive6});

Motor_Group drivetrain ({drive1,drive2,drive3, drive4,drive5,drive6});

Motor intake (17, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);

Rotation xTracking (18);
Rotation yTracking (9);

IMU inertial1 (1);
IMU inertial2 (2);
IMU inertial3 (3);

ADIPort backClaw ('A', ADI_DIGITAL_OUT);
ADIPort ploinker ('B', ADI_DIGITAL_OUT);