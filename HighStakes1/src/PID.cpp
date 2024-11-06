#include "main.h"

float rotPow = 0.0;
float linPow = 0.0;

float linKp = 4.16; // 24 in to  100%
float rotKp = 1.0;

void toPoint(float targX, float targY){
    while(1){
        rotPow = topointloop(targX, targY);
        linPow = sqrtf( powf(targX - xPos, 2.0) + powf(targY - yPos, 2.0) );

        rightDrive.move_velocity(linPow * linKp - rotPow * rotKp);
        leftDrive.move_velocity (linPow * linKp + rotPow * rotKp);

        delay(10);
    }
}