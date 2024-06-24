#include "main.h"

const float pi = 3.141592653589793;
const float xWheelDiameter = 3.25;
const float yWheelDiameter = 2.75;
const float xWheelSprocketRatio = 3.0;
const float yWheelSprocketRatio = 3.0;

float xWheelOffset = 0.0;
float yWheelOffset = 0.0;
float deltaXWheel = 0.0;
float deltaYWheel = 0.0;
float currentXPosition = 0.0;
float currentYPosition = 0.0;
float previousXPosition = 0.0;
float previousYPosition = 0.0;
float currentTheta = 0.0;
float previousTheta = 0.0;
float deltaTheta = 0.0;
float deltaXLocal = 0.0;
float deltaYLocal = 0.0;
float avgTheta = 0.0;
float deltaRLocal = 0.0;
float deltaThetaLocal = 0.0;
float modTheta = 0.0;
float deltaXGlobal = 0.0;
float deltaYGlobal = 0.0;
float xPos = 0.0;
float yPos = 0.0;


float arctan2 (float x, float y){
    float a = 0.0;
    if (x > 0){
        a = (atan(y / x));
    }
    else if (x < 0){
        a = (atan(y / x) + pi);
    }
    else if (x == 0){
        if (y > 0){
            a = (pi / 2.0);
        }
        if (y < 0){
            a = (1.5 * pi);
        }
    }
    return a;
}

void odometry(void){
    previousXPosition = 0.0;
    previousYPosition = 0.0;
    previousTheta = 0.0;

    xTracking.set_position(0.0);
    yTracking.set_position(0.0);
    inertial1.set_heading(0.0);
    inertial2.set_heading(0.0);
    inertial3.set_heading(0.0);

    while(1){
        //currentXPosition = xTracking.get_position();
        currentYPosition = yTracking.get_position();
        //currentTheta = getAngle();
        currentTheta = inertial1.get_rotation();

        //deltaXWheel = (currentXPosition - previousXPosition) * xWheelDiameter * pi / (36000.0 * xWheelSprocketRatio); //inches
        deltaYWheel = (currentYPosition - previousYPosition) * yWheelDiameter * pi / (36000.0 * yWheelSprocketRatio);
        deltaTheta = currentTheta - previousTheta; //degrees

        avgTheta = (previousTheta + currentTheta) / 2.0;

        //previousXPosition = currentXPosition;
        previousYPosition = currentYPosition;
        previousTheta = currentTheta;

        //deltaXLocal = 2 * ((deltaXWheel / deltaTheta) + xWheelOffset) * sin(deltaTheta / 2.0);
        deltaXLocal = 0.0;
        deltaYLocal = 2 * ((deltaYWheel / deltaTheta) + yWheelOffset) * sin(deltaTheta / 2.0);

        deltaRLocal = sqrt((powf(deltaXLocal, 2.0)) + powf(deltaYLocal, 2.0));
        deltaThetaLocal = arctan2(deltaXLocal, deltaYLocal);
        modTheta = deltaThetaLocal - avgTheta;

        deltaXGlobal = deltaRLocal * cos(modTheta);
        deltaYGlobal = deltaRLocal * sin(modTheta);

        xPos += deltaXGlobal;
        yPos += deltaYGlobal;

        pros::lcd::set_text(1, std::to_string(xPos));
        pros::lcd::set_text(2, std::to_string(yPos));
        pros::lcd::set_text(3, std::to_string(currentTheta));
        pros::lcd::set_text(4, std::to_string(yTracking.get_position()));

        delay(10);
    }
}