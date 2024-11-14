#include "main.h"

float linkp = 12.5 * 2.0; //just stole the toplink values
float linki = 12.5 * 0.0;
float linkd = 12.5 * 1.0;

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
float arcratek = -20.0; //tune this
float rotdir = 0.0;
float rotarcrotk = 10.0;

void rotarc(float xtar, float ytar, float ttar){
    lcd::set_text(0, "entered rotarc");
    mtar = tanf(ttar);
    xsol = (((powf(xPos, 2.0) - powf(xtar, 2.0)) / (2.0 * (yPos - ytar))) + ((yPos - ytar) / 2.0) - (xtar / mtar)) / (((xPos - xtar) / (yPos - ytar)) - (1.0 / mtar));
    ysol = -1.0 * ((xPos - xtar) / (yPos - ytar)) * (xsol - ((xPos + xtar) / 2.0)) + ((yPos + ytar) / 2.0);

    xint = (-1.0 * mtar * xtar - (xPos * ((xPos - xsol) / (yPos - ysol))) - yPos + ytar) / (-1.0 * ((xPos - xsol) / (yPos - ysol)) - mtar);
    yint = -1.0 * ((xPos - xsol) / (yPos - ysol)) * (xtar - xPos) + yPos;

    cout << "mtar: " << mtar << "\n" << "xsol: " << xsol << "\n" << "ysol: " << ysol << "\n" << "xint: " << xint << "\n" << "yint: " << yint << "\n" << "xPos: " << xPos << "\n" << "yPos: " << yPos << "\n";


    loopcount = 0;
    while (loopcount < 10){
        lcd::set_text(0, "entered loop 1");
        ttotar = (2 * (xint - xPos >= 0.0) - 1) * pi / 2.0 - atanf((yint - yPos) / (xint - xPos)); //check if condition returns boolean
        //ttotar = (2 * 1 - 1) * pi / 2.0 - atanf((yint - yPos) / (xint - xPos)); //check if condition returns boolean
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
        lcd::set_text(1, "loopcount:");
        lcd::set_text(2, std::to_string(loopcount));
        lcd::set_text(3, "xsol:");
        lcd::set_text(4, std::to_string(xsol));
        lcd::set_text(5, "ysol:");
        lcd::set_text(6, std::to_string(ysol));
        lcd::set_text(7, std::to_string(terror));
    
        //delay(1000);
    }
    rightDrive.brake();
    leftDrive.brake();
    lcd::set_text(0, "exited loop 1");
    delay(1000);

    loopcount = 0;
    prevTheta = currentTheta; //just so it's not 0 on the first loop?
    arcrotpow = 10.0;
    while (loopcount < 10){
        lcd::set_text(0, "entered loop 2");
        mtar = tanf(ttar);
        xsol = (((powf(xPos, 2.0) - powf(xtar, 2.0)) / (2.0 * (yPos - ytar))) + ((yPos - ytar) / 2.0) - (xtar / mtar)) / (((xPos - xtar) / (yPos - ytar)) - (1.0 / mtar));
        ysol = -1.0 * ((xPos - xtar) / (yPos - ytar)) * (xsol - ((xPos + xtar) / 2.0)) + ((yPos + ytar) / 2.0);
        
        xint = (-1.0 * mtar * xtar - (xPos * ((xPos - xsol) / (yPos - ysol))) - yPos + ytar) / (-1.0 * ((xPos - xsol) / (yPos - ysol)) - mtar);
        yint = -1.0 * ((xPos - xsol) / (yPos - ysol)) * (xtar - xPos) + yPos;
        arcr = sqrtf(powf(xtar - xsol, 2.0) + powf(ytar - ysol, 2.0));

        arcerr = arcr * fabsf((arctan2(xtar - xsol, ytar - ysol) - arctan2(xPos - xsol, yPos - ysol)));
        if (ttotar >= 0.0){rotdir = 1.0;}
        else{rotdir = -1.0;}
        arcint += arcerr;
        if ((fabs(arcerr) <= arcerrmin) || fabs(arcint) >= arcintmax){arcint = 0.0;}
        arcder = arcerr - prearcerr;
        prearcerr = arcerr;
        arclinpow = linkp * arcerr + linki * arcint + linkd * arcder;

        /*
        arcratetar = (ttotar - currentTheta) / arcerr;
        arcrateact = (currentTheta - prevTheta) / arcder;
        prevTheta = currentTheta;
        arcratedif = arcratetar - arcrateact;
        arcrotpow += arcratek * arcratedif;
    
        rightDrive.move_velocity(arclinpow - rotdir * arcrotpow);
        leftDrive.move_velocity(arclinpow + rotdir * arcrotpow);
        */

        //ttotar = (2 * (xint - xPos >= 0.0) - 1) * pi / 2.0 - atanf((yint - yPos) / (xint - xPos)); //check if condition returns boolean
        //ttotar = (2 * 1 - 1) * pi / 2.0 - atanf((yint - yPos) / (xint - xPos)); //check if condition returns boolean
        ttotar = (2 * (xtar - xPos >= 0.0) - 1) * pi / 2.0 - atanf((ytar - yPos) / (xtar - xPos)); //check if condition returns boolean
        terror = 0.5 * (ttotar - currentTheta);
        rotint += terror;
        if ((fabs(terror) <= rotintmin) || fabs(rotint) >= rotintmax){rotint = 0.0;}
        rotder = terror - preterror;
        preterror = terror;
        rotpow = rotkp * terror + rotki * rotint + rotkd * rotder;

        rightDrive.move_velocity(arclinpow - rotarcrotk * rotpow);
        leftDrive.move_velocity(arclinpow + rotarcrotk * rotpow);


        /*
        rightDrive.move_velocity(arclinpow);
        leftDrive.move_velocity(drive1.get_actual_velocity() * (arcr - (13.75/2.0)) / (arcr + (13.75/2.0)));
        */

        if (arcerr <= 1.0){loopcount += 1;}
        else{loopcount = 0;}
        delay(10);
        lcd::set_text(0, "loopcount2:");
        lcd::set_text(1, std::to_string(loopcount));
    }
    lcd::set_text(0, "exited loop 2");
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
    while(loopcounter1 < 10){ //!THIS WAS 50 SO IF IT IS BROKEN CHANGE IT BACK
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



float avar = 1.0;
float bvar = 1.0;
float wvar1 = 1.0;
float wvar2 = 1.0;
float lambda = 1.0;
float epsilon = 0.001;
float km = 0.5; // 0<=km<=1
float cm = 1.0; // 0<cm
float vm = 1.0; //i have no idea what this variable does
float econ = 2.718281828459045;
float alpha = 0.0;
float xloccen = 0.0;
float yloccen = 0.0;
float mu = 0.0;
float xloc = 0.0;
float yloc = 0.0;
float philoc = 0.0;
int pgralc = 0;

float arcypos(float x){
    if (x <= (-pi + bvar) / avar){return wvar1;}
    else if ((-pi + bvar) / avar < x && x <= bvar / avar){return wvar1 / 2.0 * (1 - cos(avar * x - bvar));}
    else{return 0;}
}
float arcyneg(float x){
    if (x <= (-pi + bvar) / avar){return -wvar2;}
    else if ((-pi + bvar) / avar < x && x <= bvar / avar){return -wvar2 / 2.0 * (1 - cos(avar * x - bvar));}
    else{return 0.0;}
}
float phitar(float y){
    if (0.0 <= y){return atan(-avar * sqrt((wvar1 - y) * y));}
    else{return atan(avar * sqrt(-(wvar2 + y) * y));}
}
float delphitar(float y){
    if (0.0 <= y){return -(avar * (wvar1 - 2.0 * y) * sqrt((wvar1 - y) * y)) / (2.0 * y * (1 + pow(avar, 2.0) * (wvar1 - y) * y) * (wvar1 - y));}
    else{return (avar * (wvar2 + 2.0 * y) * sqrt(-(wvar2 + y) * y)) / (2.0 * y * (1 - powf(avar, 2.0) * (wvar2 + y) * y) * (wvar2 + y));}
}
float transvel(float y, float phi){
    return -(1 - km * pow(econ, -cm * pow(y, 2.0))) * (1 - pow(econ, -cm * y * sin(phi))) / (1 + pow(econ, -cm * y * sin(phi))) * vm + km * pow(econ, -cm * pow(y, 2.0)) * vm;
}
float gyphi1(float y, float phi){
    return -lambda * (phi + atan(avar * sqrt((wvar1 - y) * y))) - ((avar * (wvar1 - 2.0 * y) * sqrt((wvar1 - y) * y)) / (2.0 * y * (1 + pow(avar, 2.0) * (wvar1 - y) * y) * (wvar1 - y))) * (transvel(y, phi) * sin(phi));
}
float gyphi2(float y, float phi){
    return -lambda * (phi - atan(avar * sqrt(-(wvar2 + y) * y))) + ((avar * (wvar2 + 2.0 * y) * sqrt(-(wvar2 + y) * y)) / (2.0 * y * (1 - pow(avar, 2.0) * (wvar2 + y) * y) * (wvar2 + y))) * (transvel(y, phi) * sin(phi));
}
float steerang(float y, float phi){
    if (0.0 <= y && y < wvar1 - epsilon){return gyphi1(y, phi);}
    else if (-wvar2 + epsilon < y && y <= 0.0){return gyphi2(y, phi);}
    else{return 0.0;} //you're cooked
}
void pgrarc(float xcen, float ycen, float arcrad, float arctheta, float gamma, float sigma){
    while(pgralc < 10){
        alpha = atan((yPos - ycen) / (xPos - xcen));
        xloccen = xcen + arcrad * cos(alpha);
        yloccen = ycen + arcrad * sin(alpha);
        mu = pi / 2.0 + alpha;

        xloc = xPos * cos(mu) + yPos * sin(mu) - xloccen * cos(mu) - yloccen * sin(mu);
        yloc = -xPos * sin(mu) + yPos * cos(mu) + xloccen * sin(mu) - yloccen * cos(mu);
        philoc = currentTheta - mu;

        rightDrive.move_voltage(12000.0 * (transvel(yloc, philoc) + steerang(yloc, philoc)));
        leftDrive.move_voltage(12000.0 * (transvel(yloc, philoc) - steerang(yloc, philoc)));

        if (yloc < 0.1){pgralc += 1;}
        else{pgralc = 0;}
        delay(10);
        lcd::set_text(0, std::to_string(transvel(yloc, philoc)));
        lcd::set_text(1, std::to_string(steerang(yloc, philoc)));
        lcd::set_text(2, std::to_string(xloccen));
        lcd::set_text(3, std::to_string(yloccen));
        lcd::set_text(4, std::to_string(philoc));
        lcd::set_text(5, std::to_string(alpha));
        lcd::set_text(6, std::to_string(mu));
        lcd::set_text(7, std::to_string(yloc));

    }
    rightDrive.brake();
    leftDrive.brake();
}