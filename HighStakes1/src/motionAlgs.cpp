#include "main.h"

float linkp = 0.0;
float linki = 0.0;
float linkd = 0.0;
float rotkp = 0.0;
float rotki = 0.0;
float rotkd = 0.0;
float terror = 0.0;
float preterror = 0.0;
float xerror = 0.0;
float yerror = 0.0;
float rotint = 0.0;
float rotder = 0.0;
float rotpow = 0.0;
float rotintmin = 1.0;
float rotintmax = 50.0;
float linerr = 0.0;
float linint = 0.0;
float linintmin = 0.0;
float linintmax = 0.0;
float linder = 0.0;
float prelinerr = 0.0;
float linpow = 0.0;
float ttotar = 0.0;

void rotlinrot(float targetx, float targety, float targett){
    while (terror >= 0.1){
        ttotar = 0.0;
        terror = targett - currentTheta;
        rotint += terror;
        if ((fabs(terror) <= rotintmin) || fabs(terror) >= rotintmax){rotint = 0.0;}
        rotder = terror - preterror;
        preterror = terror;
        rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
        rightDrive.move_velocity(-rotpow);
        leftDrive.move_velocity(rotpow);
    }
    rightDrive.brake();
    leftDrive.brake();

    while (linerr >= 0.1){
        linerr = sqrtf(powf((targetx - xPos), 2.0) + powf((targety - yPos), 2.0));
        linint += linerr;
        if ((fabs(linerr) <= linintmin) || fabs(linerr) >= linintmax){linint = 0.0;}
        linder = linerr - prelinerr;
        prelinerr = linerr;
        linpow = linkp * linerr + linki * linint + linkd * linder;
        rightDrive.move_velocity(linpow);
        leftDrive.move_velocity(linpow);
    }
    rightDrive.brake();
    leftDrive.brake();

}