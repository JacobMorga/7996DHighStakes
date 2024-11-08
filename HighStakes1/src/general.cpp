#include "main.h"
using namespace std;

float getDir(float input){
    if (input > 0){return 1;}
    else if (input < 0){return -1;}
    else{return 0;}
}

float dif12 = 0.0;
float dif23 = 0.0;
float dif13 = 0.0;
float leastdif = 0.0;
int difselect = 0;
float angle = 0.0;

const bool on = true;
const bool off = false;
const bool pressed = true;
const bool unpressed = false;

float getAngle(void){
    dif12 = fabs(inertial1.get_rotation() - inertial2.get_rotation());
    dif23 = fabs(inertial2.get_rotation() - inertial3.get_rotation());
    dif13 = fabs(inertial1.get_rotation() - inertial3.get_rotation());

    leastdif = dif12; difselect = 1;
    if (dif23 <= leastdif){leastdif = dif23; difselect = 2;}
    if (dif13 <= leastdif){leastdif = dif13; difselect = 3;}
    
    if (difselect == 1){angle = 0.5 * (inertial1.get_rotation() + inertial2.get_rotation());}
    else if (difselect == 2){angle = 0.5 * (inertial2.get_rotation() + inertial3.get_rotation());}
    else{angle = 0.5 * (inertial1.get_rotation() + inertial3.get_rotation());}

    return angle;
}

float getRangle(void){
    return pi / 180.0 * getAngle();
}

float arctan2(float x, float y){
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