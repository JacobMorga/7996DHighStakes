#include "main.h"

float linkp = 1.0;
float linki = 0.0;
float linkd = 0.0;

float rotkp = 200.0; //212.5; //150.0
float rotki = 0.0;
float rotkd = 1625.0;

float terror = 0.0;
float preterror = 0.0;
float xerror = 0.0;
float yerror = 0.0;
float rotint = 0.0;
float rotder = 0.0;
float rotpow = 0.0;
float rotintmin = 0.5; //tune
float rotintmax = 1000.0; //tune
float rotinterrmax = 10.0;
float linerr = 0.0;
float linint = 0.0;
float linintmin = 1.0; //tune
float linintmax = 50.0; //tune
float linder = 0.0;
float prelinerr = 0.0;
float linpow = 0.0;
float ttotar = 0.0;
int loopcount = 0;

bool inputvovfp = 0;

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
        if (fabs(terror) <= pi / 180.0 * 1.0){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
        lcd::set_text(0, std::to_string(xPos));
        lcd::set_text(1, std::to_string(yPos));        
        lcd::set_text(2, std::to_string(currentTheta));
        //lcd::set_text(3, std::to_string());
        lcd::set_text(4, std::to_string(terror));
        lcd::set_text(5, std::to_string(rotpow));
        lcd::set_text(6, std::to_string(ttotar));
        lcd::set_text(7, std::to_string(loopcount));
    }
    rightDrive.brake();
    leftDrive.brake();
    //lcd::set_text(7, "reset"));

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
        lcd::set_text(0, std::to_string(xPos));
        lcd::set_text(1, std::to_string(yPos));        
        lcd::set_text(2, std::to_string(180 / pi * currentTheta));
        //lcd::set_text(3, std::to_string());
        lcd::set_text(4, std::to_string(linerr));
        lcd::set_text(5, std::to_string(linpow));
        //lcd::set_text(6, std::to_string());
        lcd::set_text(7, std::to_string(loopcount));
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
float arcerrmin = 1.0; //tune
float arcintmax = 50.0; //tune
float arcder = 0.0;
float arclinpow = 0.0;
float arcrotpow = 0.0;
float arcratetar = 0.0;
float arcrateact = 0.0;
float prevTheta = 0.0;
float arcratedif = 0.0;
float arcratek = 0.0; //tune this
float rotdir = 0.0;

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

float topointloop(float targetx, float targety){
    lcd::set_text(7, "entered topointloop");
    lcd::set_text(4, std::to_string(180 / pi * terror));
    lcd::set_text(5, std::to_string(20.0 * rotpow));
    lcd::set_text(6, std::to_string(loopcount));
    lcd::set_text(7, "topointloop variables ^");

    ttotar = (2 * (targetx - xPos >= 0.0) - 1) * pi / 2.0 - atanf((targety - yPos) / (targetx - xPos)); //check if condition returns boolean
    terror = ttotar - currentTheta;
    while (terror > pi){terror -= 2.0 * pi;}
    while (terror < -pi){terror += 2.0 * pi;}
    if (dirdec == 0){
        if (terror >= 0.0){rotdir = 1;}
        else{rotdir = -1.0;}
        dirdec = 1;
    }
    //if (fabs(terror) / terror != rotdir && fabs(terror) > pi / 4.0){terror += 2.0 * pi * rotdir;}
    rotint += terror;
    //if ((fabs(terror) <= rotintmin) || fabs(rotint) >= rotintmax || fabs(terror) >= lininterrmax){rotint = 0.0;}
    if ((fabs(terror) <= pi / 180.0 * rotintmin) || fabs(terror) >= pi / 180.0 * rotinterrmax){rotint = 0.0;}
    lcd::set_text(3, std::to_string(20.0 * rotki * rotint));
    lcd::set_text(4, std::to_string(180 / pi * terror));
    rotder = terror - preterror;
    preterror = terror;
    rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
    
    if (inputvovfp == 0){
        rightDrive.move_velocity(-rotpow);
        leftDrive.move_velocity(rotpow);
    }
    else{
        rightDrive.move_voltage(20.0 * -rotpow);
        leftDrive.move_voltage(20.0 * rotpow);
    }
    
    if (fabs(terror) <= pi / 180 * 1.0){loopcount += 1;}
    else{loopcount = 0;}
    delay(10);
    return rotpow;
    lcd::set_text(7, "CHECK THIS end of topointloop");
}


float ttotara = 0.0;
float ttotarb = 0.0;
float tpl2dir = 0.0;
float ttotaraerr = 0.0;
float ttotarberr = 0.0;

float topointloop2(float targetx, float targety){
    lcd::set_text(3, "entered loop2");
    ttotara = (2 * (targetx - xPos >= 0.0) - 1) * pi / 2.0 - atanf((targety - yPos) / (targetx - xPos));
    if (ttotara >= 0.0){ttotarb = (2 * (targetx - xPos >= 0.0) - 1) * pi / 2.0 - atanf((targety - yPos) / (targetx - xPos)) - pi;}
    else{ttotarb = (2 * (targetx - xPos >= 0.0) - 1) * pi / 2.0 - atanf((targety - yPos) / (targetx - xPos)) + pi;}
    ttotaraerr = ttotara - currentTheta;
    ttotarberr = ttotarb - currentTheta;
    while (ttotaraerr > pi){ttotaraerr -= 2.0 * pi;}
    while (ttotaraerr < -pi){ttotaraerr += 2.0 * pi;}    
    while (ttotarberr > pi){ttotarberr -= 2.0 * pi;}
    while (ttotarberr < -pi){ttotarberr += 2.0 * pi;}
    if (fabs(ttotaraerr) > fabs(ttotarberr)){
        tpl2dir = -1.0;
        ttotar = ttotarb;
    }
    else{
        tpl2dir = 1.0;
        ttotar = ttotara;
    }
    terror = ttotar - currentTheta;
    while (terror > pi){terror -= 2.0 * pi;}
    while (terror < -pi){terror += 2.0 * pi;}
    

    /*
    ttotar = (2 * (targetx - xPos >= 0.0) - 1) * pi / 2.0 - atanf((targety - yPos) / (targetx - xPos)); //check if condition returns boolean
    terror = ttotar - currentTheta;
    while (terror > pi){terror -= 2.0 * pi;}
    while (terror < -pi){terror += 2.0 * pi;}
    if (dirdec == 0){
        if (terror >= 0.0){rotdir = 1;}
        else{rotdir = -1.0;}
        dirdec = 1;
    }
    if (fabs(terror) / terror != rotdir && fabs(terror) > pi / 4.0){terror += 2.0 * pi * rotdir;}
    */

    rotint += terror;
    if ((fabs(terror) <= pi / 180.0 * rotintmin) || fabs(terror) >= pi / 180.0 * rotinterrmax){rotint = 0.0;}
    rotder = terror - preterror;
    preterror = terror;
    rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;
    delay(10);
    return rotpow;
}


void facepoint (float targetx, float targety){
    lcd::set_text(7, "entered facepoint");
    loopcount = 0;
    dirdec = 0;
    while (loopcount < 10){ //loopcount < 10
        lcd::set_text(7, "entered loop to call topointloop");
        topointloop(targetx, targety);
    }
    lcd::set_text(7, "exited topointloop loop");
    rightDrive.brake();
    leftDrive.brake();
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

float rotPow = 0.0;
float toplinerr = 0.0;
float toplinint = 0.0;
float toplinder = 0.0;
float pretoplinerr = 0.0;
float toplinintmin = 1.0; //tune
float toplinintmax = 1000.0; //tune
float toplinpow = 0.0;
float toplinkp = 2.0; //tune
float toplinki = 0.0; //tune
float toplinkd = 1.0; //tune
float toplinmax = 600.0;

float linKp = 12.5; // 20
float rotKp = 2.5;
float tpright = 0.0;
float tpleft = 0.0;
float deadzonerad = 12.0; //12.0;
float dist = 0.0;
int loopcounter1 = 0;
bool inputvov = 0;
bool pauseloop = false;

void toPoint(float targX, float targY){
    loopcounter1 = 0;
    dirdec = 0;
    while(loopcounter1 < 10){
        rotPow = topointloop2(targX, targY);
        dist = sqrtf(powf(targX - xPos, 2.0) + powf(targY - yPos, 2.0));
        toplinerr = tpl2dir * dist * cos(ttotar - currentTheta);
        //toplinerr = dist * cos(ttotar - currentTheta);
        toplinint += toplinerr;
        if ((fabs(toplinerr) <= toplinintmin) || fabs(toplinerr) >= toplinintmax){toplinint = 0.0;}
        toplinder = toplinerr - pretoplinerr;
        pretoplinerr = toplinerr;
        toplinpow = toplinkp * toplinerr + toplinki * toplinint + toplinkd * toplinder;

        tpright = toplinpow * linKp - rotPow * rotKp;
        tpleft = toplinpow * linKp + rotPow * rotKp;

        if (fabs(tpright) > toplinmax || fabs(tpleft) > toplinmax){
            tpright = tpright * toplinmax / max(fabs(toplinpow * linKp - rotPow * rotKp), fabs(toplinpow * linKp + rotPow * rotKp));
            tpleft = tpleft * toplinmax / max(fabs(toplinpow * linKp - rotPow * rotKp), fabs(toplinpow * linKp + rotPow * rotKp));
        }

        if (dist < deadzonerad){
            tpright = toplinpow * linKp - (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp);
            tpleft = toplinpow * linKp + (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp);
        }

        /*
        if (fabs(tpright) > toplinmax){tpright = toplinmax * fabs(tpright) / tpright;}
        if (fabs(tpleft) > toplinmax){tpleft = toplinmax * fabs(tpleft) / tpleft;}
        */

        lcd::clear_line(3);
        lcd::set_text(4, std::to_string(toplinmax));
        lcd::set_text(5, std::to_string(tpright));
        lcd::set_text(6, std::to_string(tpleft));
        lcd::set_text(7, std::to_string(toplinerr));

        if (pauseloop == 0){
            if (inputvov == 0){
                rightDrive.move_velocity(tpright);
                leftDrive.move_velocity(tpleft);
            }
            else{
                rightDrive.move_voltage(20.0 * tpright);
                leftDrive.move_voltage(20.0 * tpleft);
            }
        }
        else{
            loopcounter1 = 0;
        }
        

        /*
        lcd::set_text(0, std::to_string(rotkp));
        lcd::set_text(1, std::to_string(rotki));
        lcd::set_text(2, std::to_string(rotkd));
        lcd::set_text(4, std::to_string(toplinkp));
        lcd::set_text(5, std::to_string(toplinki));
        lcd::set_text(6, std::to_string(toplinkd));
        lcd::set_text(7, std::to_string(toplinpow));
        */
        

        if(fabs(drive1.get_actual_velocity()) < 20.0 && fabs(drive4.get_actual_velocity() < 20.0)){loopcounter1 += 1;}
        else{loopcounter1 = 0;}

        /*
        lcd::set_text(3, std::to_string(rotPow));
        lcd::set_text(4, std::to_string(tpl2dir));
        lcd::set_text(5, std::to_string(tpright));
        lcd::set_text(6, std::to_string(tpleft));
        lcd::set_text(7, std::to_string(180.0 / pi * ttotar));
        */


        //lcd::set_text(0, std::to_string(xPos));
        //lcd::set_text(1, std::to_string(yPos));        
        //lcd::set_text(2, std::to_string(180 / pi * currentTheta));
        //lcd::set_text(3, std::to_string(tpl2dir));
        //lcd::set_text(4, std::to_string(toplinpow));
        //lcd::set_text(5, std::to_string(tpright));
        //lcd::set_text(6, std::to_string(rotPow));
        //lcd::set_text(7, std::to_string(loopcounter1));
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
}

int indeadzone = 0;
int hitspeedlimit = 0;

void toPointthe2nd(float targX, float targY){
    lcd::set_text(3, "entered topointthe2nd");
    loopcounter1 = 0;
    dirdec = 0;
    while(loopcounter1 < 50){
        lcd::set_text(3, "entered while loop");
        rotPow = topointloop2(targX, targY);
        lcd::set_text(3, "exited loop2");
        dist = sqrtf(powf(targX - xPos, 2.0) + powf(targY - yPos, 2.0));
        toplinerr = tpl2dir * dist * cos(ttotar - currentTheta);
        //toplinerr = dist * cos(ttotar - currentTheta);
        toplinint += toplinerr;
        if ((fabs(toplinerr) <= toplinintmin) || fabs(toplinerr) >= toplinintmax){toplinint = 0.0;}
        toplinder = toplinerr - pretoplinerr;
        pretoplinerr = toplinerr;
        toplinpow = toplinkp * toplinerr + toplinki * toplinint + toplinkd * toplinder;

        tpright = toplinpow * linKp - rotPow * rotKp;
        tpleft = toplinpow * linKp + rotPow * rotKp;

        if (dist < deadzonerad){
            tpright = toplinpow * linKp - (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp);
            tpleft = toplinpow * linKp + (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp);
            indeadzone = 1;

            if (fabs(tpright) > toplinmax || fabs(tpleft) > toplinmax){
                tpright = tpright * toplinmax / max(fabs(toplinpow * linKp - (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp)), fabs(toplinpow * linKp + (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp)));
                tpleft = tpleft * toplinmax / max(fabs(toplinpow * linKp - (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp)), fabs(toplinpow * linKp + (powf(dist, 4.0) / powf(deadzonerad, 4.0)) * (rotPow * rotKp)));
                hitspeedlimit = 2;
            }
            else{hitspeedlimit = 0;}
        }
        else{
            indeadzone = 0;
            if (fabs(tpright) > toplinmax || fabs(tpleft) > toplinmax){
                tpright = tpright * toplinmax / max(fabs(toplinpow * linKp - rotPow * rotKp), fabs(toplinpow * linKp + rotPow * rotKp));
                tpleft = tpleft * toplinmax / max(fabs(toplinpow * linKp - rotPow * rotKp), fabs(toplinpow * linKp + rotPow * rotKp));
                hitspeedlimit = 2;
            }
            else{hitspeedlimit = 0;}
        }
    


        /*
        if (fabs(tpright) > toplinmax){tpright = toplinmax * fabs(tpright) / tpright;}
        if (fabs(tpleft) > toplinmax){tpleft = toplinmax * fabs(tpleft) / tpleft;}
        */

        lcd::set_text(4, std::to_string(indeadzone + hitspeedlimit));
        lcd::set_text(5, std::to_string(tpright));
        lcd::set_text(6, std::to_string(tpleft));
        lcd::set_text(7, std::to_string(toplinerr));

        if (pauseloop == 0){
            if (inputvov == 0){
                rightDrive.move_velocity(tpright);
                leftDrive.move_velocity(tpleft);
            }
            else{
                rightDrive.move_voltage(20.0 * tpright);
                leftDrive.move_voltage(20.0 * tpleft);
            }
        }
        else{
            loopcounter1 = 0;
        }
        

        /*
        lcd::set_text(0, std::to_string(rotkp));
        lcd::set_text(1, std::to_string(rotki));
        lcd::set_text(2, std::to_string(rotkd));
        lcd::set_text(4, std::to_string(toplinkp));
        lcd::set_text(5, std::to_string(toplinki));
        lcd::set_text(6, std::to_string(toplinkd));
        lcd::set_text(7, std::to_string(toplinpow));
        */
        

        if(fabs(drive1.get_actual_velocity()) < 20.0 && fabs(drive4.get_actual_velocity() < 20.0)){loopcounter1 += 1;}
        else{loopcounter1 = 0;}

        /*
        lcd::set_text(3, std::to_string(rotPow));
        lcd::set_text(4, std::to_string(tpl2dir));
        lcd::set_text(5, std::to_string(tpright));
        lcd::set_text(6, std::to_string(tpleft));
        lcd::set_text(7, std::to_string(180.0 / pi * ttotar));
        */


        //lcd::set_text(0, std::to_string(xPos));
        //lcd::set_text(1, std::to_string(yPos));        
        //lcd::set_text(2, std::to_string(180 / pi * currentTheta));
        //lcd::set_text(3, std::to_string(tpl2dir));
        //lcd::set_text(4, std::to_string(toplinpow));
        //lcd::set_text(5, std::to_string(tpright));
        //lcd::set_text(6, std::to_string(rotPow));
        //lcd::set_text(7, std::to_string(loopcounter1));
        delay(10);
    }
    rightDrive.brake();
    leftDrive.brake();
    lcd::set_text(3, "exited pid");
}