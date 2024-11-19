#include "main.h"

const float rotKP = 1.0; //*tune this
const float rotKI = 0.0; //*tune this
const float rotKD = 0.0; //*tune this
const float tErrorMin = 1.0; //*tune after tuning rotKI
const float tIntMax = 1000.0; //*tune after tuning rotKI

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
        tError = tTar - tPos;
        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_voltage(tPow);
        leftDrive.move_voltage(-tPow);
        if (fabs(tError) <= 0.01){faceHeadingLoops += 1;} //about 0.5 degrees
        else{faceHeadingLoops = 0;}
        delay(10);
    }
}

void facePoint(float xTar, float yTar){
    facePointLoops = 0;
    while (facePointLoops < 10){
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = tTarget - tPos;
        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_voltage(tPow);
        leftDrive.move_voltage(-tPow);
        if (fabs(tError) <= 0.01){faceHeadingLoops += 1;} //about 0.5 degrees
        else{faceHeadingLoops = 0;}
        delay(10);
    }
}

void turnBy(float angle){
    turnByLoops = 0;
    tTarget = tPos + angle;
    while (turnByLoops < 10){
        tError = tTarget - tPos;
        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_voltage(tPow);
        leftDrive.move_voltage(-tPow);
        if (fabs(tError) <= 0.01){faceHeadingLoops += 1;} //about 0.5 degrees
        else{faceHeadingLoops = 0;}
        delay(10);
    }
}

const float linKP = 1.0; //*tune this
const float linKI = 0.0; //*tune this
const float linKD = 0.0; //*tune this
const float lErrorMin = 1.0; //*tune after tuning linKI
const float lIntMax = 1000.0; //*tune after tuning linKI

float lError = 0.0;
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
    while (linearLoops < 10){
        currentPos = yTracking.get_position() * yWheelDiameter * pi / 36000.0 - initPos;
        lError = distance - currentPos;
        lInt += lError;
        //if (fabs(lError) <= lErrorMin || fabs(lInt) >= lIntMax){lInt = 0.0;} //*tune linKI first
        lDer = lError - lPrevError;
        lPrevError = lError;
        lPow = linKP * lError + linKI * lInt + linKD * lDer;
        rightDrive.move_voltage(lPow);
        leftDrive.move_voltage(lPow);
        if (fabs(lError) <= 0.5){linearLoops += 1;}
        else {linearLoops = 0;}
        delay(10);
    }
}

void rotLinRot(float xTar, float yTar, float tTar){
    facePoint(xTar, yTar);
    //then to the point somehow
    faceHeading(tTar);
}

void toPoint(float xTar, float yTar){}

void rotArc(float xTar, float yTar, float tTar){}