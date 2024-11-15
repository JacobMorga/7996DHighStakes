#ifndef GENERAL_H // Prevents file from getting included multiple times
#define GENERAL_H

using namespace pros;

extern Controller controller;

extern Motor drive1;
extern Motor drive2;
extern Motor drive3;
extern Motor drive4;
extern Motor drive5;
extern Motor drive6;
extern Motor_Group rightDrive;
extern Motor_Group leftDrive;
extern Motor_Group drivetrain;
extern Motor intake;

extern Rotation xTracking;
extern Rotation yTracking;

extern IMU inertial1;
extern IMU inertial2;
extern IMU inertial3;

extern ADIPort backClaw;
extern ADIPort ploinker;

extern const float pi;
extern const bool pressed;
extern const bool unpressed;
extern const bool on;
extern const bool off;

template <typename anyVar>
extern anyVar getDir(anyVar input);
extern float arctan2(float x, float y);

template <typename anyVar>
anyVar printToBrain(int lineNum, anyVar input);

template <typename anyVar>
anyVar printToConsole(string name, anyVar input);

template <typename anyVar>
anyVar printPointToConsole(string name, anyVar x, anyVar y);

#endif