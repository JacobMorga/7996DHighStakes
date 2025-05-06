#include "main.h"
using namespace pros;
float goalDist = 8.0;
float ringStackDist = 4.0;
bool runThisCodeShort = 0;
int missedCounter = 0;
bool ladderTouch = 0;
bool skipAS = 0;

void bluePos(){
    teamColor = COLOR_BLUE;
    missedCounter = 0;
    transit(26);
    toPoint(-24.0, 32.0, 0, 1, 12000.0); //rush goal //!y31.5
    facePoint(-24.0, 48.0, 1, 12000.0); //make sure you point at it when you put the wall mech down
    transit(6); //wall mech score
    delay(250);
    faceHeading(250.0, 1, 12000.0); //pull line goal back farther //right 0, forward 90
    transit(0);
    faceAway(0.0, 24.0, 1, 12000.0); //face away from second goal
    toPoint(0.0, 24.0, 1, 0, 8000.0); //toward second goal (autoclamp)
    //if(backClaw.get_value() == 0){rightClearer.set_value(1); delay(1000); rightClearer.set_value(0); backClaw.set_value(1); delay(250);}
    toPoint(10.0, 36.0, 0, 1, 12000.0); //to ladder rings
    facePoint(24.0, 48.0, 0, 12000.0); //face rings under ladder
    leftClearer.set_value(1);
    delay(150); //let clearer fall
    checkPseudoVel = 0;
    toPoint(-12.0, 6.0, 1, 1, 12000.0); //come out of ladder with ring on clearer
    checkPseudoVel = 1;
    whatToRun = 6; //lift clearer during turn
    //delay(20);
    //leftClearer.set_value(0);
    WMForwardTarget = 1200.0;
    instantLift2 = 1; //~new
    transit(4); //~new
    //state21stop = 0;
    facePoint(-48.0, -12.0, 0, 12000.0); //face corner before you let the clearer up //!a little high so it comes in a little more angled?
    whatToRun = 7;
    toPoint(-48.0 - 50.0, -24.0 - 50.0, 0, 1, 12000.0); //!smooth? //also this is behind the field on purpose
    toPoint(18.0, 0.0, 1, 1, 12000.0); //to alliance stake //$tune at worlds
    facePoint(24.0, -24.0, 0, 12000.0); //point to alliance stake //$tune at worlds
    toPointShortBy(24.0, -24.0, 0, 0, 19.0, 8000.0); //correct distance to stake error //$tune at worlds
    transit(6); //score alliance stake //!22
    delay(500);
    transit(0); //pull wall mech back
    delay(500); //let wall mech up before we drive away so it doesn't get caught
    toPoint(0.0, 24.0, 1, 1, 12000.0); //go to ladder backward
    toPoint(0.0, 44.0, 0, 1, 12000.0); //touch ladder base
}

void blueNeg(){
    teamColor = COLOR_BLUE;
    std::vector<coord> blueNegPath1 = {coord(0.0, 24.0), coord(12.0, 38.0)};
    std::vector<coord> blueNegPath2 = {coord(12.0, 38.0), coord(36.0, 38.0), coord(36.0, 30.0)}; //!(-36,24)->(-36,30)
    std::vector<coord> blueNegPath3 = {coord(36.0, 30.0), coord(0.0, 24.0)}; //!(-36,24)->(-36,30)
    
    toPoint(0.0, 24.0, 1, 1, 12000.0); //get goal with autoclamp //!(3, 21)
    //toPoint(-3.0, 27.0, 1, 1, 6000.0); //go just a tad slower at the end
    delay(100);
    transit(1);
    purePursuit(blueNegPath1, 12.0, 6000.0, 1); //three rings all bottom of stacks
    purePursuit(blueNegPath2, 12.0, 6000.0, 1); //finish
    purePursuit(blueNegPath3, 12.0, 9000.0, 1); //finish

    toPoint(-12.0, 36.0, 0, 1, 12000.0); //to ladder rings
    facePoint(-20.0, 48.0, 0, 12000.0); //face rings under ladder
    rightClearer.set_value(1);
    delay(150); //let clearer fall
    toPoint(12.0, 0.0, 1, 1, 12000.0); //come out of ladder with ring on clearer
    whatToRun = 8; //lift clearer during turn
    //leftClearer.set_value(0);
    facePoint(48.0, -24.0, 0, 12000.0); //face corner before you let the clearer up
    toPoint(48.0, -24.0, 0, 0, 12000.0); //intake bottom ring onto goal
    //delay(500); //let the ring in (perchance unnecessary)
    toPoint(24.0, 0.0, 1, 1, 12000.0); //leave corner
    transit(4);
    toPoint(-20.0, 3.0, 0, 1, 12000.0); //to ring stack //!weird +3y drift 💀
    toPoint(-48.0, 3.0, 0, 1, 6000.0); //intake knocked ring //!weird +3y drift 💀
    toPoint(-24.0, -3.0, 1, 1, 12000.0); //align to alliance stake
    facePoint(-24.0, -24.0, 0, 12000.0); //face alliance stake
    //toPointShortBy(24.0, -24.0, 0, 0, 20.0, 12000.0); //really aim that john
    transit(6); //score alliance stake
    delay(500); //let arm down
    transit(0);
    delay(500); //let arm up (perchance unnecessary)
    toPoint(-24.0, 6.0, 1, 1, 12000.0); //away from alliance stake so the arm doesn't get caught
    toPoint(-60.0, -12.0, 1, 0, 12000.0); //get goal over to corner=
}

void blueSolo(){}

void redPos(){
    teamColor = COLOR_RED;
    missedCounter = 0;
    transit(26);
    toPoint(24.0, 31.5, 0, 1, 12000.0); //rush goal
    facePoint(24.0, 48.0, 1, 12000.0); //make sure you point at it when you put the wall mech down
    transit(6); //wall mech score
    delay(250);
    faceHeading(-250.0, 1, 12000.0); //pull line goal back farther //right 0, forward 90
    transit(0);
    faceAway(0.0, 24.0, 1, 12000.0); //face away from second goal
    toPoint(-3.0, 21.0, 1, 0, 8000.0); //toward second goal (autoclamp) //%just pulled (-3,-3)
    //if(backClaw.get_value() == 0){rightClearer.set_value(1); delay(1000); rightClearer.set_value(0); backClaw.set_value(1); delay(250);}
    toPoint(-12.0, 38.0, 0, 1, 12000.0); //to ladder rings
    facePoint(-24.0, 48.0, 0, 12000.0); //face rings under ladder
    rightClearer.set_value(1);
    delay(150); //let clearer fall
    checkPseudoVel = 0;
    toPoint(12.0, 6.0, 1, 1, 12000.0); //come out of ladder with ring on clearer
    checkPseudoVel = 1;
    whatToRun = 8; //lift clearer during turn
    //delay(20);
    //leftClearer.set_value(0);
    WMForwardTarget = 1200.0;
    instantLift2 = 1; //~new
    transit(4); //~new
    //state21stop = 0;
    facePoint(48.0, -24.0, 0, 12000.0); //face corner before you let the clearer up
    whatToRun = 7;
    toPoint(48.0, -24.0, 0, 1, 12000.0); //!smooth?
    delay(500); //let that ring in?
    toPoint(-18.0, 0.0, 1, 1, 12000.0); //to alliance stake //$tune at worlds
    facePoint(-26.0, -24.0, 0, 12000.0); //point to alliance stake //$tune at worlds //!-2x?
    toPointShortBy(-26.0, -24.0, 0, 0, 19.0, 8000.0); //correct distance to stake error //$tune at worlds //!-2x?
    transit(6); //score alliance stake
    delay(500);
    transit(0); //pull wall mech back
    delay(500); //let wall mech up before we drive away so it doesn't get caught
    toPoint(0.0, 24.0, 1, 1, 12000.0); //go to ladder backward
    toPoint(0.0, 44.0, 0, 1, 12000.0); //touch ladder base
}

void redNeg(){
    teamColor = COLOR_RED;
    std::vector<coord> redNegPath1 = {coord(0.0, 24.0), coord(-12.0, 38.0)};
    std::vector<coord> redNegPath2 = {coord(-12.0, 38.0), coord(-36.0, 38.0), coord(-36.0, 30.0)}; //!(-36,24)->(-36,30)
    std::vector<coord> redNegPath3 = {coord(-36.0, 30.0), coord(0.0, 24.0)}; //!(-36,24)->(-36,30)
    
    toPoint(-3.0, 22.0, 1, 1, 12000.0); //get goal with autoclamp
    toPoint(3.0, 27.0, 1, 1, 6000.0); //go just a tad slower at the end
    delay(100);
    transit(1);
    purePursuit(redNegPath1, 12.0, 6000.0, 1); //three rings all bottom of stacks
    purePursuit(redNegPath2, 12.0, 6000.0, 1); //finish
    purePursuit(redNegPath3, 12.0, 12000.0, 1); //finish

    toPoint(12.0, 36.0, 0, 1, 12000.0); //to ladder rings
    facePoint(20.0, 48.0, 0, 12000.0); //face rings under ladder
    leftClearer.set_value(1);
    delay(150); //let clearer fall
    toPoint(-12.0, 0.0, 1, 1, 12000.0); //come out of ladder with ring on clearer
    whatToRun = 6; //lift clearer during turn
    //leftClearer.set_value(0);
    facePoint(-48.0, -24.0, 0, 12000.0); //face corner before you let the clearer up
    toPoint(-48.0, -24.0, 0, 0, 12000.0); //intake bottom ring onto goal
    //delay(500); //let the ring in (perchance unnecessary)
    toPoint(-24.0, 0.0, 1, 1, 12000.0); //leave corner
    transit(4);
    toPoint(20.0, 3.0, 0, 1, 12000.0); //to ring stack //!weird +3y drift 💀
    toPoint(42.0, 3.0, 0, 1, 6000.0); //intake knocked ring //!weird +3y drift 💀
    toPoint(24.0, -3.0, 1, 1, 12000.0); //align to alliance stake
    facePoint(24.0, -24.0, 0, 12000.0); //face alliance stake
    //toPointShortBy(24.0, -24.0, 0, 0, 20.0, 12000.0); //really aim that john
    transit(6); //score alliance stake
    delay(1000); //let arm down
    transit(0);
    delay(500); //let arm up (perchance unnecessary)
    toPoint(24.0, 6.0, 1, 1, 12000.0); //away from alliance stake so the arm doesn't get caught
    toPoint(60.0, -12.0, 1, 0, 12000.0); //get goal over to corner

}

void redSolo(){}

void blueNegAS(){
    teamColor = COLOR_BLUE;
    std::vector<coord> blueNegASPath1 = {coord(0.0, 24.0), coord(12.0, 38.0)};
    std::vector<coord> blueNegASPath2 = {coord(12.0, 38.0), coord(30.0, 38.0), coord(30.0, 30.0)};
    std::vector<coord> blueNegASPath3 = {coord(30.0, 30.0), coord(0.0, 24.0)};

    if(skipAS == 0){
        transit(6); //score alliance stake preload
        delay(700);
        transit(1);
        drivetrain.move_voltage(-3000.0);
        delay(500);
        drivetrain.move_voltage(0.0);
    }
    
    toPoint(0.0, 26.0, 1, 1, 12000.0); //get goal with autoclamp
    delay(100);
    purePursuit(blueNegASPath1, 12.0, 6000.0, 1); //three rings all bottom of stacks
    purePursuit(blueNegASPath2, 12.0, 6000.0, 1); //finish
    purePursuit(blueNegASPath3, 12.0, 12000.0, 1); //finish

    //drivetrain.brake();
    //delay(1000); //just let the sorting sorta sort itself sorted of a sort

    toPoint(-13.0, 37.0, 0, 1, 12000.0); //to ladder rings
    facePoint(-24.0, 48.0, 0, 12000.0); //face rings under ladder
    rightClearer.set_value(1);
    delay(150); //let clearer fall
    checkPseudoVel = 0;
    toPoint(12.0, 0.0, 1, 1, 12000.0); //come out of ladder with ring on clearer
    checkPseudoVel = 1;
    whatToRun = 8; //lift clearer during turn
    //leftClearer.set_value(0);
    facePoint(48.0, -30.0, 0, 12000.0); //face corner before you let the clearer up //!y-24 intentional overangle
    toPoint(48.0, -24.0, 0, 1, 12000.0); //intake bottom ring onto goal //~just smoothed
    //delay(500); //let the ring in (perchance unnecessary)
    toPoint(24.0, 0.0, 1, 1, 12000.0); //leave corner
    //transit(4);
    if(ladderTouch){
        toPoint(-24.0, 24.0, 0, 1, 6000.0); //touch ladder
    }
    else{
        toPoint(-20.0, 3.0, 0, 1, 12000.0); //to ring stack //!weird +3y drift 💀
        toPoint(-60.0, 3.0, 0, 1, 6000.0); //intake knocked ring //!weird +3y drift 💀
        toPoint(-72.0, -12.0, 1, 0, 12000.0); //get goal over to corner
    }
}

void redNegAS(){
    teamColor = COLOR_RED;
    std::vector<coord> redNegASPath1 = {coord(0.0, 24.0), coord(-12.0, 38.0)};
    std::vector<coord> redNegASPath2 = {coord(-12.0, 38.0), coord(-30.0, 38.0), coord(-30.0, 30.0)};
    std::vector<coord> redNegASPath3 = {coord(-30.0, 30.0), coord(0.0, 24.0)};

    if(skipAS == 0){
        transit(6); //score alliance stake preload
        delay(700);
        transit(1);
        drivetrain.move_voltage(-3000.0);
        delay(500);
        drivetrain.move_voltage(0.0);
    }
    
    toPoint(0.0, 26.0, 1, 1, 12000.0); //get goal with autoclamp
    delay(100);
    purePursuit(redNegASPath1, 12.0, 6000.0, 1); //three rings all bottom of stacks
    purePursuit(redNegASPath2, 12.0, 6000.0, 1); //finish
    purePursuit(redNegASPath3, 12.0, 12000.0, 1); //finish

    //drivetrain.brake();
    //delay(1000); //just let the sorting sorta sort itself sorted of a sort

    toPoint(13.0, 37.0, 0, 1, 12000.0); //to ladder rings
    facePoint(24.0, 48.0, 0, 12000.0); //face rings under ladder
    leftClearer.set_value(1);
    delay(150); //let clearer fall
    checkPseudoVel = 0;
    toPoint(-6.0, 12.0, 1, 1, 12000.0); //come out of ladder with ring on clearer //$change blueneg
    checkPseudoVel = 1;
    whatToRun = 9; //lift clearer during turn
    //leftClearer.set_value(0);
    facePoint(-48.0, -30.0, 0, 12000.0); //face corner before you let the clearer up //!y-24 intentional overangle
    toPoint(-48.0, -24.0, 0, 1, 12000.0); //intake bottom ring onto goal //~just smoothed
    //delay(500); //let the ring in (perchance unnecessary)
    toPoint(-24.0, 0.0, 1, 1, 12000.0); //leave corner
    //transit(4);
    if(ladderTouch){
        toPoint(24.0, 24.0, 0, 1, 6000.0); //touch ladder
    }
    else{
        toPoint(20.0, 3.0, 0, 1, 12000.0); //to ring stack //!weird +3y drift 💀
        toPoint(60.0, 3.0, 0, 1, 6000.0); //intake knocked ring //!weird +3y drift 💀
        toPoint(72.0, -12.0, 1, 0, 12000.0); //get goal over to corner
    }
}








void bluePosAWP(){
    teamColor = COLOR_BLUE;
    std::vector<coord> bpaPath1 = {coord(-36.0, -12.0), coord(-36.0, 24.0), coord(-24.0, 36.0)};

    toPoint(-36.0, 33.0, 0, 1, 12000.0);
    faceHeading(75.0, 1, 12000.0);
    rightClearer.set_value(1);
    delay(500);
    smoothFPError = 10.0;
    faceHeading(90.0, 1, 6000.0);
    toPoint(-36.0, 0.0, 1, 1, 8000.0);
    rightClearer.set_value(0);
    faceHeading(110.0, 1, 12000.0); //get clearer unstuck
    transit(3);
    faceHeading(45.0, 1, 12000.0); //point around rushed goal
    toPoint(-18.0, 12.0, 0, 1, 12000.0); //move around rushed goal
    toPoint(-22.0, 26.0, 0, 1, 12000.0); //intake bottom red ring
    delay(500); //possession
    faceAway(0.0, 24.0, 0, 12000.0); //face back to goal
    toPoint(8.0, 24.0, 1, 1, 12000.0); //to goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    delay(250);

    facePoint(24.0, 0.0, 1, 12000.0); //face ring
    toPoint(21.0, 6.0, 0, 1, 12000.0); //toward red ring
    toPoint(33.0, -6.0, 0, 1, 6000.0); //intake bottom red ring slowly
    delay(500); //possession
    toPoint(39.0, -12.0, 0, 1, 12000.0); //intake top blue ring
    toPoint(19.0, -3.5, 1, 1, 12000.0); //align for wall stake //?backward //!22 in red
    transit(0);
    faceHeading(-90.0, 0, 12000.0); //face wall stake //try slower if necessary
    transit(6);
    delay(750); //scoring
    drivetrain.move_voltage(8000.0);
    delay(500);

    if(runThisCodeShort == false){ //awp
        transit(1); //intake to climb ladder
        delay(100);
        toPoint(xPos, 0.0, 1, 1, 12000.0); //away from stake
        toPoint(24.0, 24.0, 0, 0, 12000.0); //touch ladder
    }

    else if (runThisCodeShort == true){ //max
        transit(1);
        delay(100);
        toPoint(-36.0, -18.0, 0, 0, 12000.0); //jam intake in corner to maybe get a ring or just set up for corner
    }
}

void blueNegAWP(){ 
    teamColor = COLOR_BLUE;
    specialIntake = 0;

    std::vector<coord> bnaPath1 = {coord(36.0, 38.0), coord(0.0, 38.0), coord(0.0, 24.0), coord(-24.0, 0.0)};

    transit(6); //alliance stake
    delay(750);
    transit(0);
    toPoint(1.0, 27.0, 1, 1, 7000.0); //old grab goal
    //toPoint(3.0, 30.0, 1, 1, 10000.0); //from red grab goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    toPoint(33.0, 24.0, 0, 0, 12000.0); //first ring stack all the way in
    delay(500);
    toPoint(33.0, 36.0, 0, 1, 12000.0); //align ring cube
    purePursuit(bnaPath1, 24.0, 6000.0, 0); //get ring cube and go to top blue ring
    if (runThisCodeShort == false){transit(3);}
    toPoint(-30.0,-6.0,0,0,12000);
    delay(750);
    toPoint(-24.0, -6.0, 1, 1, 12000.0); //put goal behind

    if (runThisCodeShort == false){ //solo awp
        backClaw.set_value(0);
        toPoint(-24.0, 6.0, 0, 1, 12000.0); //away from goal
        toPoint(-51.0, 27.0, 1, 1, 12000.0); //second goal
        backClaw.set_value(1);
        delay(100);
        transit(1); //load held ring
        delay(1000);
        toPoint(-42.0, 30.0, 0, 0, 12000.0); //touch ladder (try backward?)
    }
    else{ //short 3/4 awp
        toPoint(6.0, 24.0, 1, 1, 12000.0); //backward to not interfere
        transit(0);
        toPoint(-9.0, 33.0, 0, 0, 12000.0); //touch rung with zip ties
        //facePoint(-24.0, 24.0, 0, 12000.0);
        //toPoint(24.0, 24.0, 0, 0, 8000.0);
    }
}

float savedKP = 0.0;
float savedKI = 0.0;
float savedKD = 0.0;

void redPosAWP(){
    teamColor = COLOR_RED;
    std::vector<coord> rpaPath1 = {coord(36.0, -12.0), coord(36.0, 24.0), coord(24.0, 36.0)};
    savedKP = TProtKP;
    savedKI = TProtKI;
    savedKD = TProtKD;

    toPoint(36.0, 30.5, 0, 1, 12000.0);
    faceHeading(145.0, 1, 12000.0);
    rightClearer.set_value(1);
    delay(500);
    smoothFPError = 10.0;
    faceHeading(90.0, 1, 6000.0);
    toPoint(36.0, 0.0, 1, 1, 8000.0);
    rightClearer.set_value(0);
    delay(100);
    transit(3);
    toPoint(22.0, 26.0, 0, 1, 12000.0); //intake bottom red ring
    delay(500); //possession
    faceAway(0.0, 24.0, 0, 12000.0); //face back to goal
    toPoint(-8.0, 24.0, 1, 1, 12000.0); //to goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    delay(250);

    facePoint(-24.0, 0.0, 1, 12000.0); //face ring
    toPoint(-21.0, 6.0, 0, 1, 12000.0); //toward blue ring
    toPoint(-33.0, -6.0, 0, 1, 6000.0); //intake bottom blue ring slowly
    delay(500); //possession
    toPoint(-39.0, -6.0, 0, 1, 12000.0); //intake top red ring
    toPoint(-22.0, -4.5, 1, 1, 12000.0); //align for wall stake //?backward
    transit(0);
    faceHeading(-90.0, 0, 12000.0); //face wall stake //try slower if necessary
    transit(6);
    delay(750); //scoring
    drivetrain.move_voltage(8000.0);
    delay(500);

    if(runThisCodeShort == false){ //awp
        transit(1);  //intake to climb ladder
        delay(100);
        toPoint(xPos, 0.0, 1, 1, 12000.0); //away from stake
        toPoint(-24.0, 24.0, 0, 0, 12000.0); //touch ladder
    }

    else if (runThisCodeShort == true){ //max
        transit(1);
        delay(100);
        toPoint(36.0, -18.0, 0, 0, 12000.0); //jam intake in corner to maybe get a ring or just set up for corner
    }
}

void redNegAWP(){
    teamColor = COLOR_RED;
    specialIntake = 0;

    std::vector<coord> rnaPath1 = {};
    std::vector<coord> rnaPath2 = {};
    std::vector<coord> rnaPath3 = {coord(-36.0, 38.0), coord(0.0, 38.0), coord(0.0, 24.0), coord(24.0, 0.0)};
    std::vector<coord> rnaPath4 = {coord(-24.0,0.0), coord(-30.0,-6.0)};
    bezierCurve(coord(-3.2, 19.2), coord(-18.0, 8.6), coord(-20.1, -3.2), coord(-20.4, -12.0), coord(-0.1, -17.8), 30, rnaPath1);
    bezierCurve(coord(4.0, 20.0), coord(-25.0, 12.0), coord(-60.0, 18.0), coord(-60., 50.0), coord(-14.0, 44.0), 100, rnaPath2);

    transit(6); //alliance stake
    delay(750);
    transit(0);
    toPoint(-3.0, 30.0, 1, 1, 10000.0); //grab goal //!7000
    backClaw.set_value(1);
    delay(100);
    transit(1);
    toPoint(-33.0, 24.0, 0, 0, 12000.0); //first ring stack all the way in //!
    delay(500);
    toPoint(-33.0, 36.0, 0, 1, 12000.0); //align ring cube //!
    purePursuit(rnaPath3, 24.0, 6000.0, 0); //get ring cube and go to top red ring
    if (runThisCodeShort == false){transit(3);}
    toPoint(30.0,-6.0,0,0,12000);
    delay(750);
    toPoint(24.0, -6.0, 1, 1, 12000.0); //put goal behind

    if (runThisCodeShort == false){ //entire solo awp
        backClaw.set_value(0);
        toPoint(24.0, 6.0, 0, 1, 12000.0); //away from goal
        toPoint(51.0, 27.0, 1, 1, 12000.0); //second goal
        backClaw.set_value(1);
        delay(100);
        transit(1); //load held ring
        delay(1000);
        toPoint(42.0, 30.0, 0, 0, 12000.0); //touch ladder (try backward?)
    }
    else{ //short 3/4 awp
        toPoint(-6.0, 24.0, 1, 1, 12000.0); //backward to not interfere
        transit(0);
        toPoint(9.0, 33.0, 0, 0, 12000.0); //touch rung with zip ties
        //facePoint(24.0, 24.0, 0, 12000.0);
        //toPoint(24.0, 24.0, 0, 0, 8000.0);
    }
}

void bluePosMax(){
    teamColor = COLOR_BLUE;
    std::vector<coord> bpaPath1 = {coord(-36.0, -12.0), coord(-36.0, 24.0), coord(-24.0, 36.0)};

    toPoint(-36.0, 33.0, 0, 1, 12000.0);
    faceHeading(75.0, 1, 12000.0);
    rightClearer.set_value(1);
    delay(500);
    smoothFPError = 10.0;
    faceHeading(90.0, 1, 6000.0);
    toPoint(-36.0, 0.0, 1, 1, 8000.0);
    rightClearer.set_value(0);
    faceHeading(110.0, 1, 12000.0); //get clearer unstuck
    transit(3);
    faceHeading(45.0, 1, 12000.0); //point around rushed goal
    toPoint(-18.0, 12.0, 0, 1, 12000.0); //move around rushed goal
    toPoint(-22.0, 26.0, 0, 1, 12000.0); //intake bottom red ring
    delay(500); //possession
    faceAway(0.0, 24.0, 0, 12000.0); //face back to goal
    toPoint(8.0, 24.0, 1, 1, 12000.0); //to goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    delay(250);

    //facePoint(24.0, 0.0, 1, 12000.0); //face ring
    //toPoint(21.0, 6.0, 0, 1, 12000.0); //toward red ring
    //toPoint(33.0, -6.0, 0, 1, 6000.0); //intake bottom red ring slowly
    //delay(500); //possession
    //toPoint(39.0, -12.0, 0, 1, 12000.0); //intake top blue ring
    //toPoint(19.0, -3.5, 1, 1, 12000.0); //align for wall stake //?backward //!22 in red
    //transit(0);
    //faceHeading(-90.0, 0, 12000.0); //face wall stake //try slower if necessary
    //transit(6);
    //delay(750); //scoring
    //drivetrain.move_voltage(8000.0);
    //delay(500);

    //if(runThisCodeShort == false){ //awp
    //    transit(1); //intake to climb ladder
    //    delay(100);
    //    toPoint(xPos, 0.0, 1, 1, 12000.0); //away from stake
    //    toPoint(24.0, 24.0, 0, 0, 12000.0); //touch ladder
    //}

    //else if (runThisCodeShort == true){ //max
    //    transit(1);
    //    delay(100);
    //    toPoint(-36.0, -18.0, 0, 0, 12000.0); //jam intake in corner to maybe get a ring or just set up for corner
    //}

    faceHeading(0.0, 0, 12000.0);
    toPoint(12.0, 36.0, 0, 0, 12000.0);
}

void blueNegMax(){ 
    teamColor = COLOR_BLUE;
}

void redPosMax(){
    teamColor = COLOR_RED;
    std::vector<coord> rpaPath1 = {coord(36.0, -12.0), coord(36.0, 24.0), coord(24.0, 36.0)};
    savedKP = TProtKP;
    savedKI = TProtKI;
    savedKD = TProtKD;

    toPoint(36.0, 30.5, 0, 1, 12000.0);
    faceHeading(145.0, 1, 12000.0);
    rightClearer.set_value(1);
    delay(500);
    smoothFPError = 10.0;
    //faceHeading(90.0, 1, 6000.0);
    toPoint(36.0, 0.0, 1, 1, 8000.0);
    rightClearer.set_value(0);
    delay(100);
    transit(3);
    toPoint(22.0, 26.0, 0, 1, 12000.0); //intake bottom red ring
    delay(500); //possession
    faceAway(0.0, 24.0, 0, 12000.0); //face back to goal
    toPoint(-8.0, 24.0, 1, 1, 12000.0); //to goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    delay(250);

    facePoint(-24.0, 0.0, 1, 12000.0); //face ring
    toPoint(-21.0, 6.0, 0, 1, 12000.0); //toward blue ring
    toPoint(-33.0, -6.0, 0, 1, 6000.0); //intake bottom blue ring slowly
    delay(500); //possession
    toPoint(-39.0, -6.0, 0, 1, 12000.0); //intake top red ring
    toPoint(-20.0, -4.5, 1, 1, 12000.0); //align for wall stake //?backward
    transit(0);
    faceHeading(-90.0, 0, 12000.0); //face wall stake //try slower if necessary
    transit(6);
    delay(750); //scoring
    drivetrain.move_voltage(8000.0);
    delay(500);


    transit(1);
    delay(100);
    toPoint(36.0, -18.0, 0, 0, 12000.0); //jam intake in corner to maybe get a ring or just set up for corner
}

void redNegMax(){
    teamColor = COLOR_RED;
    specialIntake = 0;

    std::vector<coord> rnaPath1 = {};
    std::vector<coord> rnaPath2 = {};
    std::vector<coord> rnaPath3 = {coord(-36.0, 38.0), coord(0.0, 38.0), coord(0.0, 24.0), coord(24.0, 0.0)};
    std::vector<coord> rnaPath4 = {coord(-24.0,0.0), coord(-30.0,-6.0)};
    bezierCurve(coord(-3.2, 19.2), coord(-18.0, 8.6), coord(-20.1, -3.2), coord(-20.4, -12.0), coord(-0.1, -17.8), 30, rnaPath1);
    bezierCurve(coord(4.0, 20.0), coord(-25.0, 12.0), coord(-60.0, 18.0), coord(-60., 50.0), coord(-14.0, 44.0), 100, rnaPath2);

    transit(6); //alliance stake
    delay(750);
    transit(0);
    toPoint(-3.0, 30.0, 1, 1, 10000.0); //grab goal //!7000
    backClaw.set_value(1);
    delay(100);
    transit(1);
    toPoint(-33.0, 24.0, 0, 0, 12000.0); //first ring stack all the way in //!
    delay(500);
    toPoint(-33.0, 36.0, 0, 1, 12000.0); //align ring cube //!
    purePursuit({coord(-36.0, 38.0), coord(0.0, 38.0), coord(0.0, 24.0)}, 24.0, 6000.0, 0); //get ring cube and go to top red ring
    if (runThisCodeShort == false){transit(3);}
    //toPoint(30.0,-6.0,0,0,12000);
    delay(750);
    //toPoint(24.0, -6.0, 1, 1, 12000.0); //put goal behind

    toPoint(-6.0, 24.0, 1, 1, 12000.0); //backward to not interfere
    transit(0);
    toPoint(9.0, 33.0, 0, 0, 12000.0); //touch rung with zip ties
}

void skills1(){
    teamColor = COLOR_RED;
    instantLift = 1;
    std::vector<coord> sPath1 = {};
    std::vector<coord> sPath2 = {};
    std::vector<coord> sPath3 = {};
    std::vector<coord> sPath4 = {};
    std::vector<coord> sPath5 = {};
    bezierCurve(coord(-24.0, 0.0), coord(24.0, -48.0), coord(24.0, 48.0), coord(-60.0, 10.0), coord(-48.0, 100.0), 100, sPath1);
    bezierCurve(coord(0.0, 48.0), coord(-60.0, 48.0), coord(-48.0, 24.0), coord(-48.0, 0.0), coord(-48.0, -12.0), 100, sPath2);

    transit(6); //score preload on alliance stake
    delay(500);
    transit(1); //start intaking and put wall mech down
    delay(350);
    toPointShortBy(-24.0, 0.0, 1, 1, goalDist, 10000.0); //reverse into first goal
    backClaw.set_value(1); //grab first goal
    delay(100);
    purePursuit(sPath1, 24.0, 9000.0, 0); //intake three rings
    transit(4); //put last ring into wall mech
    delay(100); // added by jacob
    toPoint(-36.0, 48.0, 1, 1, 10000.0); //reverse back to wall stake
    transit(17); //only intake on bottom
    toPoint(-60.0, 48.0, 0, 0, 6000.0); //score wall stake
    transit(4);
    while(comboState != 7){delay(10);}
    delay(1000); // added by jacob
    transit(1);
    toPoint(-36.0, 48.0, 1, 1, 10000.0);
    purePursuit(sPath2, 24.0, 10000.0, 1);
    toPoint(-60.0, 0.0, 0, 1, 10000.0);
    toPoint(-66.0, -18.0, 1, 1, 10000.0);
    backClaw.set_value(0);
    toPointShortBy(24.0, 0.0, 1, 0, goalDist, 12000.0);
    backClaw.set_value(1);  
}

void skills2(){

    savedKP = TProtKP;
    savedKI = TProtKI;
    savedKD = TProtKD;

    //* to edit next
    //*first corner end bezier lift intake topoints
    //*switch bezier pps to segment pps when necessary
    //*finish actual pathing
    //& implement during pp event (zone?)
    //& two kinds of smooth pp (add one to finish path without slowing down)

    teamColor = COLOR_RED;
    //!instantLift = 1;
    std::vector<coord> sPathA = {};
    std::vector<coord> sPathB = {};
    std::vector<coord> sPathC = {};
    std::vector<coord> sPathD = {};
    std::vector<coord> sPathE = {};
    std::vector<coord> sPathF = {coord(-24.0, 24.0), coord(48.0, 96.0)};
    std::vector<coord> sPathG = {coord(64.0, -14.0), coord(38.0, 66.0), coord(72.0, 48.0)};
    std::vector<coord> sPathH = {coord(60.0, 48.0), coord(32.0, 80.0), coord(-22.0, 104.0), coord(-60.0, 116.0)};
    std::vector<coord> sPathI = {};
    bezierCurve(coord(-14.0, -11.0), coord(-48.0, 25.0), coord(15.0, 37.0), coord(93.0, 152.0), coord(61.0, 96.0), 100, sPathA);
    bezierCurve(coord(0.0, 96.0), coord(-48.0, 44.0), coord(-48.0, 200.0), coord(-116.0, 80.0), coord(-48.0, 96.0), 100, sPathB);
    bezierCurve(coord(-48.0, 72.0), coord(-24.0, 48.0), coord(-24.0, 6.0), coord(-60.0, 50.0), coord(-86.0, 45.0), 100, sPathC);
    bezierCurve(coord(24.0, 0.0), coord(-36.0, -9.0), coord(66.0, 66.0), coord(48.0, 42.0), coord(48.0, -6.0), 100, sPathD); //!y5 -18
    //bezierCurve(coord(-24.0, 99.0), coord(-32.0, 12.0), coord(-88.0, 190.0), coord(-60.0, 15.0), coord(-48.0, 24.0), 100, sPathI);
    //bezierCurve(coord(-24.0, 72.0), coord(-40.0, 80.0), coord(-82.0, 80.0), coord(-64.0, 32.0), coord(-48.0, 33.0), 100, sPathI);
    bezierCurve(coord(-24.0, 72.0), coord(-45.0, 85.0), coord(-66.0, 70.0), coord(-64.0, 32.0), coord(-48.0, 33.0), 100, sPathI);

    transit(6); //score alliance stake ring
    delay(750);
    transit(1);
    delay(250);
    toPoint(-27.0, 3.0, 1, 0, 12000.0); //align straight at the john
    backClaw.set_value(1);
    delay(100);

    toPoint(-24.0, 24.0, 0, 0, 12000.0); //first ring
    toPoint(0.0, 48.0, 0, 0, 12000.0); //second ring
    toPoint(24.0, 72.0, 0, 0, 12000.0); //third ring
    toPoint(30.0, 90.0, 0, 1, 12000.0); //don't get first ring stack straight on
    toPoint(48.0, 96.0, 0, 0, 12000.0); //fourth ring
    faceHeading(0.0, 1, 12000.0);
    toPoint(55.5, 96.0, 0, 0, 8000.0); //right wall ring
    toPoint(42.0, 90.0, 1, 1, 12000.0); //more space to get far ring
    toPoint(48.0, 105.0, 0, 1, 8000.0); //far ring
    delay(250);

    //toPoint(48.0, 96.0, 1, 1, 12000.0); //just see if it works without this
    faceAway(72.0, 120.0, 1, 12000.0); //ready to corner the goal
    backClaw.set_value(0); //let go early so only the goal tilts and not the robot or the goal doesnt tilt at all
    delay(250); //let goal fall and hopefully stop robot from tipping
    transit(2);
    //toPoint(66.0, 114.0, 1, 1, 12000.0); //goal in corner
    toPoint(72.0, 120.0, 1, 1, 12000.0); //goal in corner

    toPoint(42.0, 90.0, 0, 1, 12000.0); //get the goal off the claw if it's hooked on still
    toPoint(66.0, 114.0, 1, 1, 12000.0); //second push to really get it in there
    toPoint(48.0, 96.0, 0, 1, 12000.0); //leave second push
    transit(1);
    toPoint(24.0, 96.0, 0, 1, 12000.0); //forward and maybe move some rings that got in the way
    faceAway(0.0, 98.0, 0, 12000.0); //face to grab goal
    toPoint(-12.0, 98.0, 1, 1, 12000.0); //go to back middle goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    
    toPoint(-24.0, 72.0, 0, 1, 12000.0);
    facePoint(-48.0, 75.0, 1, 12000.0);
    purePursuit(sPathI, 24.0, 12000.0, 0);

    //^ added 2 in y in the following section

    facePoint(-48.0, -14.0, 0, 12000.0); //face line of rings
    toPoint(-48.0, 2.0, 0, 0, 12000.0); //first straight line ring
    toPoint(-48.0, -14.0, 0, 0, 12000.0); //second straight line ring
    //toPoint(-48.0, -14.0, 0, 1, 8000.0); //line of 2 or 3 rings
    delay(500);

    faceHeading(135.0, 0, 12000.0); //face last ring
    toPoint(xPos - 6.0, yPos + 6.0, 0, 1, 12000.0); //last ring but it holds it in the intake
    transit(3);
    faceAway(-72.0, -22.0, 1, 12000.0); //point goal into corner so we can let go early
    backClaw.set_value(0);
    toPoint(-66.0, -16.0, 1, 1, 12000.0); //goal in corner
    //toPoint(-48.0, 2.0, 0, 1, 12000.0); //move for second push
    //toPoint(-66.0, -16.0, 1, 1, 12000.0); //goal in corner second
    toPoint(-48.0, 2.0, 0, 1, 12000.0); //leave so it doesnt hit when we spin away

    toPoint(-12.0, 6.0, 1, 1, 12000.0);//! 7 in y  //toward other goal backward
    faceAway(24.0, 6.0, 0, 12000.0); ////! 7 in y face back to goal actually
    toPoint(30.0, 6.0, 1, 1, 8000.0); //! 7 in y //get other goal backward cause there's no bottom intake ring
    backClaw.set_value(1);
    delay(250);
    transit(1);
    toPoint(12.0, 6.0, 0, 1, 12000.0);//! 7 in y  //avoid end of path
    purePursuit(sPathD, 20.0, 8000.0, 0); //finish bottom right corner goal
    
    toPoint(45.0, -13.0, 0, 1, 8000.0); //second to last ring on this goal //!-3x
    delay(500);
    facePoint(60.0, 2.0, 1, 12000.0); //face other ring there yk the one
    toPoint(60.0, 2.0, 0, 1, 12000.0); //wall mech ring 
    toPoint(60.0, 50.0, 0, 1, 12000.0); //redundancy ring
    toPoint(48.0, 2.0, 1, 1, 12000.0);
    faceAway(72.0, -22.0, 1, 12000.0); //align goal
    delay(500); //make sure rings go on
    backClaw.set_value(0); //early, remember bruh
    transit(2);
    toPoint(66.0, -16.0, 1, 1, 12000.0); //score goal
    toPoint(48.0, 2.0, 0, 1, 12000.0); //forward to double push
    toPoint(66.0, -16.0, 1, 1, 12000.0); //push goal again
    delay(500);

    //^ end of 2 in y section

    transit(0);
    purePursuit({coord(72.0, -24.0), coord(-24.0, 72.0), coord(0.0, 96.0)}, 24.0, 12000.0, 1); //last corner
    transit(2);
    purePursuit({coord(24.0, 108.0), coord(-24.0, 111.0), coord(-70.0, 120.0)}, 24.0, 12000.0, 0); //last corner 
    toPoint(-24.0, 96.0, 1, 1, 12000.0); //out
    faceHeading(45.0, 1, 12000.0); //try again if missed
    toPoint(-24.0, 108.0, 0, 1, 12000.0); //try again if missed
    toPoint(-66.0, 114.0, 0, 1, 12000.0); //back in and try again if it missed
    toPoint(24.0, 84.0, 1, 1, 12000.0); //back away from corner pushed blue goal
    transit(4);
    facePoint(48.0, 66.0, 1, 12000.0);
    toPoint(54.0, 60.0, 0, 0, 12000.0); //intake lone ring into wall mech (its shifted down by us )
    toPoint(42.0, 53.0, 1, 0, 12000.0); //just align the wall stake bro
    transit(6);
    facePoint(66.0, 53.0, 0, 12000.0); //point to wall stake //!49 in y
    toPoint(60.0, 53.0, 0, 1, 4000.0); //score on wall stake //!-4x //!49 in y
}

void loadPaths(){

}

void autonTesting(){
    //backClaw.set_value(1);
	//delay(500);
	//transit(1);
	//colorSorting = 0;

    /*
	std::vector<coord> pathA = {coord(0.01, 0.01), coord(-24.0, 24.0)};
	std::vector<coord> pathB = {};
	std::vector<coord> pathC = {coord(-24.0, 24.0), coord(-96.0, 96.0)};
	std::vector<coord> pathD = {coord(0.01, 0.01), coord(-48.0, 48.0)};
	std::vector<coord> pathE = {coord(-48.0, 48.0), coord(-96.0, 96.0)};

	//figure 8
	bezierCurve(coord(-24.0, 24.0), coord(-96.0, 96.0), coord(-120.0, 72.0), coord(-96.0, 0.0), coord(-72.0, 24.0), 100, pathA);
	bezierCurve(coord(-72.0, 24.0), coord(0.0, 96.0), coord(24.0, 72.0), coord(0.0, 0.0), coord(-24.0, 24.0), 100, pathB);

	//horseshoe
	//bezierCurve(coord(0.0, 72.0), coord(0.0, 73.0), coord(0.0, 96.0), coord(-23.0, 96.0), coord(-24.0, 96.0), 20, pathA);
	//pathA.push_back(coord(-72.0, 96.0));
	//bezierCurve(coord(-72.0, 96.0), coord(-73.0, 96.0), coord(-96.0, 96.0), coord(-96.0, 73.0), coord(-96.0, 72.0), 20, pathA);
	//pathA.push_back(coord(-96.0, 0.0));

	purePursuit(pathA, 24.0, 12000.0, 1);
	purePursuit(pathB, 24.0, 12000.0, 1);
	purePursuit(pathC, 24.0, 12000.0, 0);

	//purePursuit(pathD, 36.0, 12000.0, 1);
	//purePursuit(pathE, 36.0, 12000.0, 0);

	drivetrain.brake();
    while(controller.get_digital(DIGITAL_DOWN) == 0){delay(10);}
    for(string item : graphingPoints){
        std::cout << item << "\n";
        delay(1);
    }
    */
    
    /*
    faceHeading(0.0, 0, 12000.0);
    delay(2000);
    faceHeading(90.0, 0, 12000.0);
    delay(2000);
    faceHeading(180.0, 0, 12000.0);
    delay(2000);
    faceHeading(270.0, 0, 12000.0);
    delay(2000);
    faceHeading(-90.0, 0, 12000.0);
    delay(2000);
    faceHeading(360.0, 0, 12000.0);
    delay(2000);
    faceHeading(0.0, 0, 12000.0);
    delay(2000);
    */

    /*
    toPoint(36.0, 33.0, 0, 1, 12000.0);
    faceHeading(150.0, 1, 12000.0);
    rightClearer.set_value(1);
    delay(100);
    faceHeading(90.0, 1, 12000.0);
    toPoint(36.0, 0.0, 1, 0, 12000.0);
    */

   //calculateOffsets();
   runColorCalibration();
}