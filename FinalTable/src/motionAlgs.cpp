#include "main.h"


/** motionAlgs.cpp - Motion Algorithms 
 * 
 * This file contains motion algorithms to control the robot's movement on the field.
 * Functions include:
 * - facePoint: Aligns the robot to a specific point on the field. Useful for targeting game objects on matter where you are.
 * - toPoint: Moves the robot to a specific point on the field. Rotates and drives at the same time.
 * - toPointShortBy: Moves the robot to a point offset by a certain distance. Useful for lining up at a certain radius from an object. (High Stakes wall stakes)
 * - faceAway: Aligns the robot to face away from a specific point. (Reverse of toPoint)
 * - faceHeading: Aligns the robot to a specific heading.
 */

float linKP = 550.0;//500.0;
float linKI = 3.0;//1.0;
float linKD = 5000.0;//5000.0;

float rotKP = 25000.0; //15000.0;
float rotKI = 1000.0; //750.0;
float rotKD = 175000.0; //100000.0; //106250.0; 

const float tErrorMin = -1.0; //*tune after tuning rotKI
const float tErrorMax = 5.0 / 180.0 * pi; //*tune after tuning rotKI
const float tIntMax = 2000.0; //*tune after tuning rotKI

float tError = 0.0;
float tInt = 0.0;
float tDer = 0.0;
float tPrevError = 0.0;
float tPow = 0.0;
float tTarget = 0.0; //this is a variable while tTar is an input
int exitLoops = 0; //universal
bool pseudoVelSwitch = 0;
float pseudoVelLimit = 1.0;
float pseudoRotVelLimit = 1.0;
float maxAcceptableRotError = 1.5;
float smoothFPError = 5.0;
bool checkPseudoVel = 1;

void facePoint(float xTar, float yTar, bool fpSmooth, float speedLimit){
    pseudoVelSwitch = 0;
    exitLoops = 0;
    while (exitLoops < 25){
        tPos = (pi / 2.0) - tPos;
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = normAngle(tTarget - tPos);
        tInt += tError;
        if (fabs(tError) >= tErrorMax || fabs(tInt) >= tIntMax){tInt = 0.0;}
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        if(fabs(tPow) > speedLimit){tPow = getDir(tPow) * speedLimit;}
        rightDrive.move_voltage(tPow); 
        leftDrive.move_voltage(-tPow);

        if(pseudoVelSwitch == 0 && fabs(pseudoRotVel) > pseudoRotVelLimit){pseudoVelSwitch = 1;}

        if (fabs(tError / pi * 180.0) <= maxAcceptableRotError || (pseudoVelSwitch == 1 && fabs(pseudoRotVel) < pseudoRotVelLimit)){exitLoops += 1;}
        else if(fpSmooth == 1 && fabs(tError / pi * 180.0) < smoothFPError){exitLoops += 25;}
        else{exitLoops = 0;}

        delay(10);
    }
    drivetrain.brake();
    //lcd::set_text(7, "freaky time");
}

const float lErrorMin = 0.25; 
const float lIntMax = 1000.0;

float lError = 0.0;
float lInt = 0.0;
float lDer = 0.0;
float lPrevError = 0.0;
float lPow = 0.0;

int linearLoops = 0;
float initPos = 0.0;
float currentPos = 0.0;

int toPointLoops = 0;
float maxDist = 0.0;
float dist = 0.0;
float rightPow = 0.0;
float leftPow = 0.0;
const float tWeightK = 0.257; //*tune this
float tWeightA = 0.0;
float tWeightB = 0.0;
float tWeight = 0.0;
float deadZoneRadius = 12.0;
float ttotara = 0.0;
float ttotarb = 0.0;
float ttotaraerr = 0.0;
float ttotarberr = 0.0;
float tpl2dir = 0.0;
float ttotar = 0.0;
float distLimit = 5.0;

float TProtKP = 30000.0; //best so far 15000.0;
float TProtKI = 0.0; //best so far 0.0;
float TProtKD = 500000.0; //best so far 500000.0;

float offsetDist = 0.0;
float angleToTarget = 0.0;
bool updateTargets = 0;
float xTarShortInput = 0.0;
float yTarShortInput = 0.0;

bool TPSB1 = 1;

void toPoint(float xTar, float yTar, float reversed, bool smooth, float powerLimit){

    xTar *= fieldRatio;
    yTar *= fieldRatio;
    toPointLoops = 0;
    tInt = 0.0;
    tDer = 0.0;
    tPrevError = 0.0;
    lInt = 0.0;
    lDer = 0.0;
    lPrevError = 0.0;
    pseudoVelSwitch = 0;
    pseudoVelocity = 0.0;
    maxDist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0));
    while (toPointLoops < 50){

        if(updateTargets){
            xTar = xTarShortInput - offsetDist * cos(angleToTarget);
            yTar = yTarShortInput - offsetDist * sin(angleToTarget);
        }

        tPos = (pi / 2.0) - tPos + pi * reversed; //! UN FRANK THIS BAD
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = normAngle(tTarget - tPos);
        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = TProtKP * tError + TProtKI * tInt + TProtKD * tDer;

        dist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0));
        lError = (1.0 - 2.0 * reversed) * dist * cos(tError);
        lInt += lError;
        if (fabs(lError) <= lErrorMin || fabs(lInt) >= lIntMax){lInt = 0.0;} //*tune linKI first
        lDer = lError - lPrevError;
        lPrevError = lError;
        lPow = linKP * lError + linKI * lInt + linKD * lDer;

        tWeightA = (1.0 + sqrt(1.0 + 4.0 / tWeightK)) / 2.0;
        tWeightB = 1.0 / (tWeightK * (tWeightA - 1.0));
        tWeight = 1.0 / (tWeightK * (1.0 - (dist / maxDist) - tWeightA)) + tWeightB;
        //if (dist < deadZoneRadius){tWeight = 0.0;}

        rightPow = lPow + tWeight * tPow;
        leftPow = lPow - tWeight * tPow;

        if (fabs(rightPow) >= powerLimit || fabs(leftPow) >= powerLimit){
            if (fabs(rightPow) > fabs(leftPow)){
                rightPow = getDir(lPow + tWeight * tPow) * powerLimit;
                leftPow = getDir(lPow - tWeight * tPow) * fabs(powerLimit * (lPow - tWeight * tPow) / (lPow + tWeight * tPow));
            }
            else{
                rightPow = getDir(lPow + tWeight * tPow) * fabs(powerLimit * (lPow + tWeight * tPow) / (lPow - tWeight * tPow));
                leftPow = getDir(lPow - tWeight * tPow) * powerLimit;
            }
        }

        //if(reversed == 1.0){rightPow *= -1.0; leftPow *= -1.0;}
        //rightPow = 0;
        //leftPow = 0;
        rightDrive.move_voltage(rightPow);
        leftDrive.move_voltage(leftPow);

        //lcd::set_text(4, std::to_string(tPos));
        //lcd::set_text(6, std::to_string(rightPow));
        //lcd::set_text(7, std::to_string(tError));

        if(pseudoVelSwitch == 0 && pseudoVelocity > pseudoVelLimit){pseudoVelSwitch = 1;}

        /* //!i took this out between switching from positive to negative corner
        if(smooth == 1 && exitDis > distance(xPos,yPos,xTar,yTar)){
            toPointLoops += 100; //immediately exits loop
        }
        */

        if (dist < distLimit || (pseudoVelSwitch == 1 && pseudoVelocity < pseudoVelLimit && checkPseudoVel == 1)){
            if (smooth == 0){toPointLoops += 1;} // +=1
            else{toPointLoops += 10;} //+=10
        }
        else{toPointLoops = 0;}

        /*
        lcd::clear();
        //lcd::print(0, "%f : rightPow", rightPow);
        //lcd::print(1, "%f : leftPow", leftPow);
        lcd::print(0, "%f : xTar", xTar);
        lcd::print(1, "%f : yTar", yTar);
        //lcd::print(2, "%f : tPos", tPos);
        lcd::print(4, "%f : lError", lError);
        lcd::print(5, "%f : lPow", lPow / 1000.0);
        lcd::print(6, "%f : tError", tError);
        lcd::print(7, "%f : tPow", tWeight * tPow / 1000.0);
        */

       //toPointLoops = 0;
        delay(10);
    }
    drivetrain.brake();
    updateTargets = 0;
}

void toPointShortBy(float xTar, float yTar, float reversed, bool smooth, float offsetDist, float powerLimit){
    float angleToTarget = arctan2(xTar - xPos, yTar - yPos);
    xTarShortInput = xTar;
    yTarShortInput = yTar;
    if(TPSB1){updateTargets = 0;}
    else{updateTargets = 1;}
    toPoint(xTar - offsetDist * cos(angleToTarget), yTar - offsetDist * sin(angleToTarget), reversed, smooth, powerLimit);
}

void faceAway(float xTar, float yTar, bool fpSmooth, float speedLimit){
    facePoint(2.0 * xPos - xTar, 2.0 * yPos - yTar, fpSmooth, speedLimit);
}

void faceHeading(float tTar, bool fpSmooth, float speedLimit){
    tTar *= pi / 180.0;
    facePoint(xPos + 96.0 * cos(tTar), yPos + 96.0 * sin(tTar), fpSmooth, speedLimit);
}