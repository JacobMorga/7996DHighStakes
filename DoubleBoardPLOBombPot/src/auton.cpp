#include "main.h"

using namespace pros;

void bluePositive(){
    rightDrive.move_voltage(6000.0);
    leftDrive.move_voltage(6000);
    delay(1000);
    rightDrive.brake();
    leftDrive.brake();
}

void blueNegative(){

}

void redPositive(){

}

void redNegative(){

}

void skills(){

}