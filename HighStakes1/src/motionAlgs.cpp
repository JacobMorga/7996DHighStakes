#include "main.h"

float linkp = 0.0;
float linki = 0.0;
float linkd = 0.0;

float rotkp = 150.0;
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
        ttotar = (2 * (targetx - xPos >= 0.0) - 1) * pi / 2.0 - atanf((targety - yPos) / (targetx - xPos)); //check if condition returns boolean
        terror = ttotar - currentTheta;
        rotint += terror;
        if ((fabs(terror) <= rotintmin) || fabs(terror) >= rotintmax){rotint = 0.0;}
        rotder = terror - preterror;
        preterror = terror;
        rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
        rightDrive.move_velocity(-rotpow);
        leftDrive.move_velocity(rotpow);
        if (fabs(terror) <= 1.0){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
    delay(1000);

    loopcount = 0;
    while (loopcount < 10){
        //currently only considers distance to target point in final speed calculation; no angular correction
        linerr = sqrtf(powf((targetx - xPos), 2.0) + powf((targety - yPos), 2.0));
        linint += linerr;
        if ((fabs(linerr) <= linintmin) || fabs(linerr) >= linintmax){linint = 0.0;}
        linder = linerr - prelinerr;
        prelinerr = linerr;
        linpow = linkp * linerr + linki * linint + linkd * linder;
        rightDrive.move_velocity(linpow);
        leftDrive.move_velocity(linpow);
        if (fabs(linerr) <= 0.1){loopcount += 1;}
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
        if (fabs(terror) <= 1.0){loopcount += 1;}
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
float arcerr = 0.0;
float arcr = 0.0;
float arcint = 0.0;
float prearcerr = 0.0;
float arcerrmin = 0.0;
float arcintmax = 0.0;
float arcder = 0.0;
float arclinpow = 0.0;
float arcrotpow = 0.0;
float arcratetar = 0.0;
float arcrateact = 0.0;
float prevTheta = 0.0;
float arcratedif = 0.0;
float arcratek = 0.0; //tune this

void rotarc(float xtar, float ytar, float ttar){
    mtar = tanf(ttar);
    xsol = (((powf(xPos, 2.0) - powf(xtar, 2.0)) / (2.0 * (yPos - ytar))) + ((yPos - ytar) / 2.0) - (xtar / mtar)) / (((xPos - xtar) / (yPos - ytar)) - (1.0 / mtar));
    ysol = -1.0 * ((xPos - xtar) / (yPos - ytar)) * (xsol - ((xPos + xtar) / 2.0)) + ((yPos + ytar) / 2.0);

    xint = (-1.0 * mtar * xtar - (xPos * ((xPos - xsol) / (yPos - ysol))) - yPos + ytar) / (-1.0 * ((xPos - xsol) / (yPos - ysol)) - mtar);
    yint = -1.0 * ((xPos - xsol) / (yPos - ysol)) * (xtar - xPos) + yPos;

    loopcount = 0;
    while (loopcount < 10){
        ttotar = (2 * (xint - xPos >= 0.0) - 1) * pi / 2.0 - atanf((yint - yPos) / (xint - xPos)); //check if condition returns boolean
        terror = ttotar - currentTheta;
        rotint += terror;
        if ((fabs(terror) <= rotintmin) || fabs(rotint) >= rotintmax){rotint = 0.0;}
        rotder = terror - preterror;
        preterror = terror;
        rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
        rightDrive.move_velocity(-rotpow);
        leftDrive.move_velocity(rotpow);
        if (fabs(terror) <= 1.0){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
    delay(1000);

    loopcount = 0;
    prevTheta = currentTheta; //just so it's not 0 on the first loop?
    while (loopcount < 10){
        mtar = tanf(ttar);
        xsol = (((powf(xPos, 2.0) - powf(xtar, 2.0)) / (2.0 * (yPos - ytar))) + ((yPos - ytar) / 2.0) - (xtar / mtar)) / (((xPos - xtar) / (yPos - ytar)) - (1.0 / mtar));
        ysol = -1.0 * ((xPos - xtar) / (yPos - ytar)) * (xsol - ((xPos + xtar) / 2.0)) + ((yPos + ytar) / 2.0);
        arcr = sqrtf(powf(xtar - xsol, 2.0) + powf(ytar - ysol, 2.0));

        arcerr = arcr * fabsf((arctan2(xtar - xsol, ytar - ysol) - arctan2(xPos - xsol, yPos - ysol)));
        if (ttotar >= 0.0){rotdir = 1.0;}
        else{rotdir = -1.0;}
        arcint += arcerr;
        if ((fabs(arcerr) <= arcerrmin) || fabs(arcint) >= arcintmax){arcint = 0.0;}
        arcder = arcerr - prearcerr;
        prearcerr = arcerr;
        arclinpow = linkp * arcerr + linki * arcint + linkd * arcder;

        arcratetar = (ttotar - currentTheta) / arcerr;
        arcrateact = (currentTheta - prevTheta) / arcder;
        prevTheta = currentTheta;
        arcratedif = arcratetar - arcrateact;
        arcrotpow += arcratek * arcratedif;

        rightDrive.move_velocity(arclinpow - rotdir * arcrotpow);
        leftDrive.move_velocity(arclinpow + rotdir * arcrotpow);
        if (arcerr <= 1.0){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
    }

}


bool dirdec = 0;
float rotdir = 0.0;

float topointloop(float targetx, float targety){

    ttotar = (2 * (targetx - xPos >= 0.0) - 1) * pi / 2.0 - atanf((targety - yPos) / (targetx - xPos)); //check if condition returns boolean
    terror = ttotar - currentTheta;
    if (dirdec == 0){
        if (terror >= 0.0){rotdir = 1;}
        else{rotdir = -1.0;}
        dirdec = 1;
    }
    if (fabs(terror) / terror != rotdir && fabs(terror) > pi / 4.0){terror += 2.0 * pi * rotdir;}
    rotint += terror;
    if ((fabs(terror) <= rotintmin) || fabs(terror) >= rotintmax){rotint = 0.0;}
    rotder = terror - preterror;
    preterror = terror;
    rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
    rightDrive.move_velocity(-rotpow);
    leftDrive.move_velocity(rotpow);
    if (fabs(terror) <= 1.0){loopcount += 1;}
    else{loopcount = 0;}
    delay(10);
    return rotpow;
}

void topoint (float targetx, float targety){
    xPos = 5.0;
    yPos = -10.0;
    loopcount = 0;
    dirdec = 0;
    while (1){ //loopcount < 10
        topointloop(targetx, targety);
    }
    rightDrive.brake();
    leftDrive.brake();
    delay(1000);
}


float targetxconst = 0.0;
float targetyconst = 0.0;
void lindist(float distance){
    //currently places target point at distance ahead of current position with heading,
    //but only considers distance to target point in final speed calculation;
    //no angular correction.
    loopcount = 0;
    targetxconst = xPos + distance * cos(currentTheta);
    targetyconst = yPos + distance * sin(currentTheta);
    while (loopcount < 10){
        linerr = sqrtf(powf((targetxconst - xPos), 2.0) + powf((targetyconst - yPos), 2.0));
        linint += linerr;
        if ((fabs(linerr) <= linintmin) || fabs(linerr) >= linintmax){linint = 0.0;}
        linder = linerr - prelinerr;
        prelinerr = linerr;
        linpow = linkp * linerr + linki * linint + linkd * linder;
        rightDrive.move_velocity(linpow);
        leftDrive.move_velocity(linpow);
        if (fabs(linerr) <= 0.1){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
}

vector<vector<float>> intersectionPoints {};

float px1 = 0.0;
float py1 = 0.0;
float px2 = 0.0;
float py2 = 0.0;

float diffX = 0.0;
float diffY = 0.0;
float diffR = 0.0;
float diffD = 0.0;

float intersectionCount = 0.0;
int counter = 0;

float intX1 = 0.0;
float intX2 = 0.0;
float intY1 = 0.0;
float intY2 = 0.0;
    
vector<vector<float>> circlePathIntersection (vector<vector<float>> straightLinePath, float robotX, float robotY, float lookAheadDis){

    // move the robot center to the origin (easier math)
    for (vector<float> point : straightLinePath){

        point[0] -= robotX;
        point[1] -= robotY;
    }

    while (counter < straightLinePath.size() - 1){

        px1 = straightLinePath[counter][0];
        py1 = straightLinePath[counter][1];
        px2 = straightLinePath[counter + 1][0];
        py2 = straightLinePath[counter + 1][1];

        // begining of calculations
        diffX = px2-px1;
        diffY = py2-py1;

        diffR = sqrtf(powf(diffX, 2.0) + powf(diffY, 2.0));
        diffD = px1*py2 - px2*py1;

        intersectionCount = powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0);

        if (intersectionCount >= 0.0){ // One or two intersections

            intX1 = (diffD * diffY + fabs(diffY) / diffY * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
            intX2 = (diffD * diffY - fabs(diffY) / diffY * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);

            intY1 = (-diffD * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
            intY2 = (-diffD * diffX - fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);


            if (intX1 == intX2 && intY1 == intY2){

                if (!(intX1 > px1 && intX1 > px2) && !(intX1 < px1 && intX1 < px2)){
                    intersectionPoints.push_back({intX1, intY1, static_cast<float>(counter)});
                }
            }
            else {
                if (!(intX1 > px1 && intX1 > px2) && !(intX1 < px1 && intX1 < px2)){
                    intersectionPoints.push_back({intX1, intY1, static_cast<float>(counter)});
                }

                if (!(intX2 > px1 && intX2 > px2) && !(intX2 < px1 && intX2 < px2)){
                    intersectionPoints.push_back({intX2, intY2, static_cast<float>(counter)});
                }
            }
        }
        counter++;
    }

    return intersectionPoints;
}

vector<float> determineBestPoint (vector<vector<float>> intersections, vector<vector<float>> path){

    vector<float> lastP = intersections[intersections.size() - 1];
    vector<float> secLastP = intersections[intersections.size() - 2];

    if (lastP[2] > secLastP[2]){

        return {lastP[0], lastP[1]};
    }
    else{

        vector<float> lineEP = path[lastP[2] + 1];

        float distLP = sqrtf(powf(lineEP[0] - lastP[0], 2.0) + powf(lineEP[1] - lastP[1], 2.0));
        float distSLP = sqrtf(powf(lineEP[0] - secLastP[0], 2.0) + powf(lineEP[1] - secLastP[1], 2.0));

        if (distLP > distSLP){
            return {secLastP[0], secLastP[1]};
        }
        else{
            return {lastP[0], lastP[1]};
        }
    }
}


vector<vector<float>> path {{-2.8, 2.9},{-1.4, -3.3},{-0.59,3.77},{3.0,-5.0},{3.1,4.01}};
void PurePursuit (){

    determineBestPoint( circlePathIntersection(path, xPos, yPos, 10.0) , path );
    
}