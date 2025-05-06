#ifndef GENERAL_H // Prevents file from getting included multiple times
#define GENERAL_H

using namespace pros;

extern Controller controller;

extern Motor drive1;
extern Motor drive2;
extern Motor drive3;
extern Motor_Group rightDrive;
extern Motor drive4;
extern Motor drive5;
extern Motor drive6;
extern Motor_Group leftDrive;
extern Motor_Group drivetrain;
extern Motor intakeTop;
extern Motor intakeBottom;
extern Motor_Group intake;
extern Motor wallMech;
extern ADIPort WMPotentiometer;

extern Rotation xTracking;
extern Rotation yTracking;
extern Rotation intakeRotation;

extern IMU inertial1;
extern IMU inertial2;
extern IMU inertial3;

extern ADIPort backClaw;
extern ADIPort rightClearer;
extern ADIPort leftClearer;
extern ADIPort intakePiston;

extern Optical opticalSensor;
extern Distance intakeDistanceSensor;
extern Distance WMDistanceSensor;

extern ADILed led1; 
extern ADILed led2;
extern ADILed led3;
extern ADILed led4;

//extern ADIDigitalIn backClawLim1;
//extern ADIDigitalIn backClawLim2;

extern Distance backClawDis1;
extern Distance backClawDis2;

extern pros::ADILED stripRight;
extern pros::ADILED stripLeft;
extern std::vector<pros::ADILED> allStrips;

extern const float pi;
extern const float eConst;
extern const bool pressed;
extern const bool unpressed;
extern const bool on;
extern const bool off;
extern const float reverse;
extern const float fieldRatio;

struct coord {
    // Structure attributes
    float x;
    float y;

    // Constructors
    coord() : x(0.0), y(0.0) {} // Default constructor (empty coord defaults to 0,0)
    coord(float xIN, float yIN){ // Constructor used to create member defined at point
        x = xIN;
        y = yIN;
    }
};

extern void printAtPoint(text_format_e_t txtFmt, int x, int y, const char* text);
extern float returnSmaller (float x, float y);
extern float returnBigger (float x, float y);
extern float getDir(float input);
extern float deEed(float eNum);
extern float getAngle(void);
extern float to_float (int n);
extern float distance(float x1, float y1, float x2, float y2);
extern float pythag (float a, float b);
extern float arctan2(float x, float y);
extern float normAngle(float angle);
extern float normAngle2pi(float angle);

void printAtPoint(text_format_e_t txtFmt, int x, int y, const char* text);

extern float getAngle(void);

void motorTesting ();


extern void USETHEBUTTONS ();
extern int getR1();
extern int getR2();
extern int getL1();
extern int getL2();
extern int getA();
extern int getB();
extern int getX();
extern int getY();
extern int getUP();
extern int getDOWN();
extern int getRIGHT();
extern int getLEFT();
#endif

// ---------- LED STUFF -----------------

extern int rgbToHexInt(int r, int g, int b);
extern void shiftVector (std::vector<int>& inputV, bool direc);
extern std::vector<int> colorGradientCalc (int sc, int ec, pros::ADILED& strip);
extern std::vector<std::vector<int>> colorTravelCalc (int startColor, int endColor, pros::ADILED& strip, bool direction);
extern void colorGradient(pros::ADILED& strip, std::vector<int> colorVector);
extern void colorTravel(pros::ADILED& strip, std::vector<std::vector<int>> gradientVectors, int timeStep);
extern void colorAlternate (std::vector<int> colors, pros::ADILED& strip, int timeStep);
extern void colorPulse(int startColor, int endColor, std::vector<pros::ADILED>& strips, int timeStep);
extern int C_Blue;
extern int C_Red;
extern void DoTheLEDs ();