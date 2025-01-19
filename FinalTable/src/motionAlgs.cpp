#include "main.h"

float rotKP = 190.0; // 100.0 for straight
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
int exitLoops = 0; //universal

void facePoint(float xTar, float yTar){ //$ DONE #################################################################################################################
                                         //! check if it can turn the right direction when its more than 90 degrees an maybe tune more its not perfect
    exitLoops = 0;
    while (exitLoops < 25){
        tPos = (pi / 2.0) - tPos;
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = normAngle(tTarget - tPos);
        tInt += tError;
        if (fabs(tError) >= tErrorMax || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;
        rightDrive.move_velocity((20.0 * tPow) / 12000.0 * 600.0); //! might need to switch which one is negative
        leftDrive.move_velocity((-20.0 * tPow) / 12000.0 * 600.0);

        if (fabs(tError / pi * 180.0) <= 0.75 || pseudoVelocity <= 0.25){exitLoops += 1;} //about 0.5 degrees
        else{exitLoops = 0;}

        lcd::set_text(3, std::to_string(tError / pi * 180.0));

        delay(10);
    }
    drivetrain.brake();
    lcd::set_text(7, "freaky time");
}


float linKP = 15.0; //*tune this
const float linKI = 0.0; //*tune this
const float linKD = 10.0; //* 12.5
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

void toPoint(float xTar, float yTar, float reversed = 0.0, bool smooth = 0){
    toPointLoops = 0;
    maxDist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0));
    while (toPointLoops < 50){

        tPos = (pi / 2.0) - tPos + pi * reversed;
        tTarget = arctan2(xTar - xPos, yTar - yPos);
        tError = normAngle(tTarget - tPos);


        tInt += tError;
        //if (fabs(tError) <= tErrorMin || fabs(tInt) >= tIntMax){tInt = 0.0;} //*tune rotKI first
        tDer = tError - tPrevError;
        tPrevError = tError;
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer;

        dist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0));
        lError = (1 - 2.0 * reversed) * dist * cos(tError);
        lInt += lError;
        //if (fabs(lError) <= lErrorMin || fabs(lInt) >= lIntMax){lInt = 0.0;} //*tune linKI first
        lDer = lError - lPrevError;
        lPrevError = lError;
        lPow = linKP * lError + linKI * lInt + linKD * lDer;

        tWeightA = (1.0 + sqrt(1.0 + 4.0 / tWeightK)) / 2.0;
        tWeightB = 1.0 / (tWeightK * (tWeightA - 1.0));
        tWeight = 1.0 / (tWeightK * (1.0 - (dist / maxDist) - tWeightA)) + tWeightB;
        //if (dist < deadZoneRadius){tWeight = 0.0;}

        rightPow = lPow + tWeight * tPow;
        leftPow = lPow - tWeight * tPow;

        if (fabs(rightPow) >= 600.0 || fabs(leftPow) >= 600.0){ //! CHANGED ALL 12000 TO 600 FOR RPM INSTEAD OF VOLTS
            if (fabs(rightPow) > fabs(leftPow)){
                rightPow = getDir(lPow + tWeight * tPow) * 600.0;
                leftPow = getDir(lPow - tWeight * tPow) * fabs(600.0 * (lPow - tWeight * tPow) / (lPow + tWeight * tPow));
            }
            else{
                rightPow = getDir(lPow + tWeight * tPow) * fabs(600.0 * (lPow + tWeight * tPow) / (lPow - tWeight * tPow));
                leftPow = getDir(lPow - tWeight * tPow) * 600.0;
            }
        }

        //if(reversed == 1.0){rightPow *= -1.0; leftPow *= -1.0;}

        rightDrive.move_voltage(rightPow / 600.0 * 12000.0);
        leftDrive.move_voltage(leftPow / 600.0 * 12000.0);

        lcd::set_text(4, std::to_string(tPos));
        lcd::set_text(6, std::to_string(rightPow));
        lcd::set_text(7, std::to_string(tError));

        if (dist < distLimit || (fabs(rightPow) < 100.0 && fabs(leftPow) < 100.0)){
            if (smooth == 0){toPointLoops += 1;}
            else{toPointLoops += 10;}
        }
        else{toPointLoops = 0;}

        delay(10);
    }
    drivetrain.brake();
}