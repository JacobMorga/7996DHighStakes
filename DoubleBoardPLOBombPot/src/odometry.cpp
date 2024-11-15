#include "main.h"

const float xWheelDiameter = 2.75;
const float yWheelDiameter = 2.75;

const float xWheelOffset = 1.0; //*calculate
const float yWheelOffset = 1.0; //*calculate
float deltaXWheel = 0.0;
float deltaYWheel = 0.0;
float currentXPosition = 0.0;
float currentYPosition = 0.0;
float previousXPosition = 0.0;
float previousYPosition = 0.0;
float tPos = 0.0;
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
        tPos = getAngle();

        deltaXWheel = (currentXPosition - previousXPosition) * xWheelDiameter * pi / 36000.0; //inches
        deltaYWheel = (currentYPosition - previousYPosition) * yWheelDiameter * pi / 36000.0;
        deltaTheta = tPos - previousTheta; //radians
        if (deltaTheta == 0.0){
            deltaXLocal = deltaXWheel;
            deltaYLocal = deltaYWheel;
        }
        else{
            deltaXLocal = 2.0 * ((deltaXWheel / deltaTheta) + xWheelOffset) * sin(deltaTheta / 2.0);
            deltaYLocal = 2.0 * ((deltaYWheel / deltaTheta) + yWheelOffset) * sin(deltaTheta / 2.0);
        }

        avgTheta = (previousTheta + tPos) / 2.0;

        previousXPosition = currentXPosition;
        previousYPosition = currentYPosition;

        deltaRLocal = sqrt((pow(deltaXLocal, 2.0)) + pow(deltaYLocal, 2.0));
        deltaThetaLocal = arctan2(deltaXLocal, deltaYLocal);
        modTheta = deltaThetaLocal - avgTheta;
        previousTheta = tPos;

        deltaXGlobal = deltaRLocal * cos(modTheta);
        deltaYGlobal = deltaRLocal * sin(modTheta);

        xPos += deltaXGlobal;
        yPos += deltaYGlobal;

        delay(10);
    }
}