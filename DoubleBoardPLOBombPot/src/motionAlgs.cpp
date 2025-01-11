#include "main.h"

const float rotKP = 190.0; // 205
const float rotKI = 0.0; // 150
const float rotKD = 1250.0; // 250
const float tErrorMin = -1.0; //*tune after tuning rotKI
const float tErrorMax = 5.0 / 180.0 * pi; //*tune after tuning rotKI
const float tIntMax = 2000.0; //*tune after tuning rotKI

float tError = 0.0;
float tInt = 0.0;
float tDer = 0.0;
float tPrevError = 0.0;
float tPow = 0.0;
float tTarget = 0.0; //this is a variable while tTar is an input
int faceHeadingLoops = 0; //i think these could all be one counter
int facePointLoops = 0;
int turnByLoops = 0;

void faceHeading(float tTar){
    faceHeadingLoops = 0;
    while (faceHeadingLoops < 10){
        tError = normAngle(tTar - tPos);
        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_voltage(20.0 * tPow);
        leftDrive.move_voltage(-20.0 * tPow);
        if (fabs(tError) <= 0.01){faceHeadingLoops += 1;} //about 0.5 degrees
        else{faceHeadingLoops = 0;}
        delay(10);
    }
}

int exitLoops = 0;
void faceHeading2 (float tTar){ //$ THIS IS NOT MATH HEADING IT WILL TRY TO FACE WHAT EVER
                                //$ DEGREES YOU PUT IN 0 BEING HEADING YOU CALIBRATED AT
    exitLoops = 0;
    tTar = tTar / 180.0 * pi;
    while (exitLoops < 30){

        tError = tTar - getAngle();
        tInt += tError;
        if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;}
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_voltage(-20.0 * tPow);
        leftDrive.move_voltage(-20.0 * tPow); 
        if (fabs(tError) <= 0.1){exitLoops += 1;} //about 0.5 degrees
        else{exitLoops = 0;}
        delay(10);
    }
}

int tampvarb = 0;
void facePoint(float xTar, float yTar){
    facePointLoops = 0;
    while (facePointLoops < 10){
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = normAngle(tTarget - tPos);
        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_voltage(20.0 * tPow);
        leftDrive.move_voltage(-20.0 * tPow);
        if (fabs(tError) <= 0.01){faceHeadingLoops += 1;} //about 0.5 degrees
        else{faceHeadingLoops = 0;}
        delay(10);
        tampvarb +=1;
        lcd::set_text(4, std::to_string(tPow));
        lcd::set_text(5, std::to_string(tError));
        std::cout << "freak" << tampvarb << "\n"; 
    }
}

void facePoint2(float xTar, float yTar){ //$ DONE #################################################################################################################
                                         //! chcek if it can turn the right direction when its more than 90 degrees an maybe tune more its not perfect
    facePointLoops = 0;
    while (facePointLoops < 25){
        tPos = (pi / 2.0) - tPos;
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = normAngle3(tTarget - tPos);
        tInt += tError;
        if (fabs(tError) >= tErrorMax || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        lcd::set_text(3, std::to_string(tError / pi * 180.0));
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_velocity((20.0 * tPow) / 12000.0 * 600.0); //! might need to switch which one is negative
        leftDrive.move_velocity((-20.0 * tPow) / 12000.0 * 600.0);

        if (fabs(tError / pi * 180.0) <= 0.75){facePointLoops += 1;} //about 0.5 degrees
        else{facePointLoops = 0;}
        delay(10);
    }
    drivetrain.brake();
    lcd::set_text(7, "freaky time");
}

void turnBy(float angle){
    turnByLoops = 0;
    tTarget = tPos + angle;
    while (turnByLoops < 10){
        tError = tTarget - tPos;
        tInt += tError;
        if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_voltage(20.0 * tPow);
        leftDrive.move_voltage(-20.0 * tPow);
        if (fabs(tError) <= 0.01){faceHeadingLoops += 1;} //about 0.5 degrees
        else{faceHeadingLoops = 0;}
        delay(10);
    }
}

const float linKP = 15.0; //*tune this
const float linKI = 0.0; //*tune this
const float linKD = 10.0; //* 12.5
const float lErrorMin = 1.0; //*tune after tuning linKI
const float lIntMax = 1000.0; //*tune after tuning linKI

float lError = 0;
float lInt = 0.0;
float lDer = 0.0;
float lPrevError = 0.0;
float lPow = 0.0;

int linearLoops = 0;
float initPos = 0.0;
float currentPos = 0.0;

void linear(float distance){ //no angle correction currently...
    linearLoops = 0;
    initPos = yTracking.get_position() * yWheelDiameter * pi / 36000.0;
    while (linearLoops < 1000){
        currentPos = yTracking.get_position() * yWheelDiameter * pi / 36000.0 - initPos;
        lError = distance - currentPos;
        lcd::set_text(3, std::to_string(lError));
        lInt += lError;
        //if (fabs(lError) <= lErrorMin || fabs(lInt) >= lIntMax){lInt = 0.0;} //*tune linKI first
        lDer = lError - lPrevError;
        lPrevError = lError;
        lPow = linKP * lError + linKI * lInt + linKD * lDer;
        rightDrive.move_velocity(lPow);
        leftDrive.move_velocity(lPow);
        if (fabs(lError) <= 0.5){linearLoops += 1;}
        else {linearLoops = 0;}
        delay(10);
    }
}
/*
void rotLinRot(float xTar, float yTar, float tTar){
    facePoint(xTar, yTar);
    toPoint(xTar, yTar);
    faceHeading(tTar);
}
*/

int toPointLoops = 0;
float maxDist = 0.0;
float dist = 0.0;
float rightPow = 0.0;
float leftPow = 0.0;
const float tWeightK = 10.0; //*tune this
float tWeightA = 0.0;
float tWeightB = 0.0;
float tWeight = 0.0;

void toPoint(float xTar, float yTar, float reversed = 0.0, bool smooth = 0){
    toPointLoops = 0;
    maxDist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0));
    while (toPointLoops < 10){
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = normAngle(tTarget - tPos + pi * reversed);
        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;

        dist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0));
        lError = dist * cos(tError);
        lInt += lError;
        //if (fabs(lError) <= lErrorMin || fabs(lInt) >= lIntMax){lInt = 0.0;} //*tune linKI first
        lDer = lError - lPrevError;
        lPrevError = lError;
        lPow = linKP * lError + linKI * lInt + linKD * lDer;

        tWeightA = (1.0 + sqrt(1.0 + 4.0 / tWeightK)) / 2.0;
        tWeightB = 1.0 / (tWeightK * (tWeightA - 1.0));
        tWeight = 1.0 / (tWeightK * (1.0 - (dist / maxDist) - tWeightA)) + tWeightB;

        rightPow = lPow + tWeight * tPow;
        leftPow = lPow - tWeight * tPow;

        if (fabs(rightPow) >= 12000.0 || fabs(leftPow) >= 12000.0){
            if (fabs(rightPow) > fabs(leftPow)){
                rightPow = getDir(lPow + tWeight * tPow) * 12000.0;
                leftPow = getDir(lPow - tWeight * tPow) * fabs(12000.0 * (lPow - tWeight * tPow) / (lPow + tWeight * tPow));
            }
            else{
                rightPow = getDir(lPow + tWeight * tPow) * fabs(12000.0 * (lPow + tWeight * tPow) / (lPow - tWeight * tPow));
                leftPow = getDir(lPow - tWeight * tPow) * 12000.0;
            }
        }

        rightDrive.move_voltage(rightPow);
        leftDrive.move_voltage(leftPow);

        if (dist < 0.5){
            if (smooth == 0){toPointLoops += 1;}
            else{toPointLoops += 10;}
        }
        else{toPointLoops = 0;}

        delay(10);
    }
}

float xTan = 0.0;
float yTan = 0.0;
float xCen = 0.0;
float yCen = 0.0;
float arcRad = 0.0;
/*
void rotArc(float xTar, float yTar, float tTar){
    xTan = xTar + cos(tTar);
    yTan = yTar + sin(tTar);
    xCen = ((yTar - yTan) * (pow(xPos, 2.0) + pow(yPos - yTar, 2.0)) + pow(xTar, 2.0) * (-2.0 * yPos + yTar + yTan) + 2.0 * xTar * xTan * (yPos - yTar)) / (2.0 * (xPos * (yTar - yTan) + xTar * (yTan - yPos) + xTan * (yPos - yTar)));
    yCen = (pow(xPos, 2.0) * (xTar - xTan) - 2.0 * xPos * (pow(xTar, 2.0) - xTar * xTan + yTar * (yTar - yTan)) + pow(xTar, 3.0) - pow(xTar, 2.0) * xTan + xTar * (pow(yPos, 2.0) + pow(yTar, 2.0) - 2.0 * yTar * yTan)) / (2.0 * (xPos * (yTan - yTar) + xTar * (yPos - yTan) + xTan * (yTar - yPos)));
    arcRad = sqrt(pow(xCen - xTar, 2.0) + pow(yCen - yTar, 2.0));

    
}
*/