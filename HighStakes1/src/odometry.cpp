#include "main.h"

const float pi = 3.141592653589793;
const float xWheelDiameter = 2.75;
const float yWheelDiameter = 3.25;
const float xWheelSprocketRatio = 1.0;
const float yWheelSprocketRatio = 1.0;

float xWheelOffset = 0.4786865;
float yWheelOffset = 1.8125;
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
        currentXPosition = xTracking.get_position();
        currentYPosition = yTracking.get_position();
        //currentTheta = getAngle();
        currentTheta = inertial1.get_rotation();

        deltaXWheel = (currentXPosition - previousXPosition) * xWheelDiameter * pi / (36000.0 * xWheelSprocketRatio); //inches
        deltaYWheel = (currentYPosition - previousYPosition) * yWheelDiameter * pi / (36000.0 * yWheelSprocketRatio);
        deltaTheta = currentTheta - previousTheta; //degrees
        if (deltaTheta == 0.0){
            if (rand() % 2 == 0){
                deltaTheta = 0.001;
            }
            else{
                deltaTheta = -0.001;
            }
        }

        avgTheta = (previousTheta + currentTheta) / 2.0;

        previousXPosition = currentXPosition;
        previousYPosition = currentYPosition;

        deltaXLocal = 2.0 * ((deltaXWheel / (pi / 180.0 * deltaTheta)) + xWheelOffset) * sin(pi / 180.0 * deltaTheta / 2.0);
        //deltaXLocal = 0.0;
        deltaYLocal = 2.0 * ((deltaYWheel / (pi / 180.0 * deltaTheta)) + yWheelOffset) * sin(pi / 180.0 * deltaTheta / 2.0);

        deltaRLocal = sqrt((powf(deltaXLocal, 2.0)) + powf(deltaYLocal, 2.0));
        deltaThetaLocal = 180.0 / pi * arctan2(deltaXLocal, deltaYLocal);
        modTheta = deltaThetaLocal - avgTheta - previousTheta;
        previousTheta = currentTheta;

        deltaXGlobal = deltaRLocal * cos(pi / 180.0 * modTheta);
        deltaYGlobal = deltaRLocal * sin(pi / 180.0 * modTheta);

        xPos += deltaXGlobal;
        yPos += deltaYGlobal;

        lcd::set_text(1, std::to_string(xPos));
        lcd::set_text(2, std::to_string(yPos));
        lcd::set_text(3, std::to_string(currentTheta));
        lcd::set_text(4, std::to_string(yTracking.get_position()));
        lcd::set_text(4, std::to_string(deltaXGlobal));
        lcd::set_text(4, std::to_string(deltaYGlobal));

        delay(10);
    }
}