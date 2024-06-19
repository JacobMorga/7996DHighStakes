#include "main.h"

Motor drive1 (1, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive2 (1, MOTOR_GEAR_600, true , MOTOR_ENCODER_DEGREES);
Motor drive3 (1, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1,drive2,drive3});

Motor drive4 (1, MOTOR_GEAR_600, true , MOTOR_ENCODER_DEGREES);
Motor drive5 (1, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive6 (1, MOTOR_GEAR_600, true , MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4,drive5,drive6});