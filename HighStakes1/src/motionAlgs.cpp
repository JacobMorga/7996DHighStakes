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
int loopcount = 0;

void rotlinrot(float targetx, float targety, float targett){
    loopcount = 0;
    while (loopcount < 10){
        ttotar = (2 * (targetx - xPos >= 0.0) - 1) * 90 - atanf((targety - yPos) / (targetx - xPos)); //check if condition returns boolean
        terror = ttotar - currentTheta;
        rotint += terror;
        if ((fabs(terror) <= rotintmin) || fabs(terror) >= rotintmax){rotint = 0.0;}
        rotder = terror - preterror;
        preterror = terror;
        rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
        rightDrive.move_velocity(-rotpow);
        leftDrive.move_velocity(rotpow);
        if (terror <= 1.0){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
    delay(1000);

    loopcount = 0;
    while (loopcount < 10){
        linerr = sqrtf(powf((targetx - xPos), 2.0) + powf((targety - yPos), 2.0));
        linint += linerr;
        if ((fabs(linerr) <= linintmin) || fabs(linerr) >= linintmax){linint = 0.0;}
        linder = linerr - prelinerr;
        prelinerr = linerr;
        linpow = linkp * linerr + linki * linint + linkd * linder;
        rightDrive.move_velocity(linpow);
        leftDrive.move_velocity(linpow);
        if (linerr <= 0.1){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
    delay(1000);

    loopcount = 0;
    rotint = 0.0;
    rotder = 0.0;
    preterror = 0.0;
    while (loopcount < 10){
        terror = targett - currentTheta;
        rotint += terror;
        if ((fabs(terror) <= rotintmin) || fabs(terror) >= rotintmax){rotint = 0.0;}
        rotder = terror - preterror;
        preterror = terror;
        rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
        rightDrive.move_velocity(-rotpow);
        leftDrive.move_velocity(rotpow);
        if (terror <= 1.0){loopcount += 1;}
        else{loopcount = 0.0;}
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
}



float ysol = 0.0;
float xsol = 0.0;
float mtar = 0.0;
float xint = 0.0;
float yint = 0.0;

void rotarc(float xtar, float ytar, float ttar){
    mtar = tanf(ttar);
    xsol = (((powf(xPos, 2.0) - powf(xtar, 2.0)) / (2.0 * (yPos - ytar))) + ((yPos - ytar) / 2.0) - (xtar / mtar)) / (((xPos - xtar) / (yPos - ytar)) - (1.0 / mtar));
    ysol = -1.0 * ((xPos - xtar) / (yPos - ytar)) * (xsol - ((xPos + xtar) / 2.0)) + ((yPos + ytar) / 2.0);

    xint = (-1.0 * mtar * xtar - (xPos * ((xPos - xsol) / (yPos - ysol))) - yPos + ytar) / (-1.0 * ((xPos - xsol) / (yPos - ysol)) - mtar);
    yint = -1.0 * ((xPos - xsol) / (yPos - ysol)) * (xtar - xPos) + yPos;


    loopcount = 0;
    while (loopcount < 10){
        ttotar = (2 * (xint - xPos >= 0.0) - 1) * 90 - atanf((yint - yPos) / (xint - xPos)); //check if condition returns boolean
        terror = ttotar - currentTheta;
        rotint += terror;
        if ((fabs(terror) <= rotintmin) || fabs(terror) >= rotintmax){rotint = 0.0;}
        rotder = terror - preterror;
        preterror = terror;
        rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
        rightDrive.move_velocity(-rotpow);
        leftDrive.move_velocity(rotpow);
        if (terror <= 1.0){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
    delay(1000);

    loopcount = 0;
    while (loopcount < 10){
        
    }

}