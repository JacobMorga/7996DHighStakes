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
extern Motor intakeTop;
extern Motor intakeBottom;
extern Motor_Group intake;
extern Motor wallMech;
extern ADIPotentiometer wallMechPotentiometer;

extern Rotation xTracking;
extern Rotation yTracking;

extern IMU inertial1;
extern IMU inertial2;
extern IMU inertial3;

extern ADIPort backClaw;
extern ADIPort rightClearer;
extern ADIPort leftClearer;

extern Optical opticalSensor;
extern Distance distanceSensor;

struct coordinate{

    float x;
    float y;
};

extern const float pi;
extern const bool pressed;
extern const bool unpressed;
extern const bool on;
extern const bool off;

template <typename anyVar>
extern float to_float (anyVar num);

template <typename anyVar>
anyVar returnSmaller (anyVar x, anyVar y);

template <typename anyVar>
anyVar returnBigger (anyVar x, anyVar y);

extern float getDir(float input);
extern float arctan2(float x, float y);
extern float getAngle(void);
extern float distance(float x1, float y1, float x2, float y2);
extern float pythagThisJohn (float a, float b);
extern float normAngle(float angle);
extern float normAngle2(float angle);
extern float normAngle3(float angle);
extern const float reverse;

template <typename anyVar>
void printToBrain(int lineNum, anyVar input);

template <typename anyVar>
void printToConsole(anyVar name, anyVar input);

template <typename anyVar>
void printPointToConsole(anyVar name, anyVar x, anyVar y);

void printAtPoint(text_format_e_t txtFmt, int x, int y, const char* text);

extern float getAngle(void);
extern const float xWheelDiameter;
extern const float yWheelDiameter;

void motorTesting ();



#endif