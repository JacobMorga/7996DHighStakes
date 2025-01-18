#include "main.h"

const float xWheelDiameter = 2.742382; //calculated
const float yWheelDiameter = 2.739546; //calculated

const float xWheelOffset = 3.069875; //calculated
const float yWheelOffset = -4.126149; //calculated
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
float calXDia = 0.0;
float calYDia = 0.0;

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

        deltaRLocal = sqrtf((powf(deltaXLocal, 2.0)) + powf(deltaYLocal, 2.0));
        deltaThetaLocal = arctan2(deltaXLocal, deltaYLocal);
        modTheta = deltaThetaLocal - avgTheta;
        previousTheta = tPos;

        deltaXGlobal = deltaRLocal * cos(modTheta);
        deltaYGlobal = deltaRLocal * sin(modTheta);

        xPos += deltaXGlobal;
        yPos += deltaYGlobal;

        //lcd::set_text(0, std::to_string(xPos));
        //lcd::set_text(1, std::to_string(yPos));
        //lcd::set_text(2, std::to_string(tPos / pi * 180.0));

        /*
        calXDia = 120.0 / (currentXPosition / 36000.0) / pi;
        calYDia = 120.0 / (currentYPosition / 36000.0) / pi;
        
        lcd::set_text(4, std::to_string(calXDia));
        lcd::set_text(5, std::to_string(calYDia));
        */

        
        //lcd::set_text(4, std::to_string(deltaXLocal));
        //lcd::set_text(5, std::to_string(deltaYLocal));
        //lcd::set_text(6, std::to_string(deltaXGlobal));
        //lcd::set_text(7, std::to_string(deltaYGlobal));
        

        delay(10);
    }
}


void calculateOffsets(){
    xTracking.reset();
    yTracking.reset();
    inertial1.reset();
    inertial2.reset();
    inertial3.reset();
    xTracking.set_position(0.0);
    yTracking.set_position(0.0);
    inertial1.set_heading(0.0);
    inertial2.set_heading(0.0);
    inertial3.set_heading(0.0);

    delay(3000);
    rightDrive.move_voltage(6000.0);
    leftDrive.move_voltage(-6000.0);
    delay(12000);
    float xAccrued = xTracking.get_position();
    float yAccrued = yTracking.get_position();
    float angleTurned = getAngle();
    float xRadius = xAccrued / 100.0 / 360.0 * pi * xWheelDiameter / angleTurned;
    float yRadius = yAccrued / 100.0 / 360.0 * pi * yWheelDiameter / angleTurned;
    lcd::set_text(0, std::to_string(xRadius));
    lcd::set_text(1, std::to_string(yRadius));
    delay(500);
    rightDrive.brake();
    leftDrive.brake();
}