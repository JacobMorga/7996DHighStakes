#include "main.h"

using namespace pros;
//!YOU ARE NOT ALLOWED TO CHANGE  A N Y  OF THESE
float goalDist = 6.0;
float cornerDist = 18.0;
float ringStackDist = 9.0;
float clearingDist = 12.0;
float wallStakeDist = 10.0;
//!FORBIDDEN ZONE ^^

bool redAWP = 1;
bool blueAWP = 1;

void bluePositive(){ //!simply copied from red side and x coordinates negatived
    teamColor = COLOR_BLUE;
    TPSB1 = 1; //don't update
    toPoint(xPos, 30.0, 1, 1, 0.0, 12000.0); //dodge ring
    faceAway(-24.0, 30.0); //face goal backward //!36 //?try (24.0, 36.0) to arc into goal (might require increasing y val of previous call)
    toPointShortBy(-24.0, 48.0, 1, 0, goalDist + 3.75, 5000.0); //goal rush
    backClaw.set_value(1);
    delay(250);
    comboState = 1; //intake, wall mech idle
    leftClearer.set_value(1);
    toPoint(-36.0, -3.5, 0, 1, 0.0, 12000.0); //stick the corner clearer in
    facePoint(0.0, yPos); //clear rings
    toPoint(0.0, -12.0, 0, 1, 0.0, 6000.0); //intake all rings
    leftClearer.set_value(0);
    toPoint(-36.0, -18.0, 1, 1, 6.0, 12000.0); //go back to corner for goal drop
    backClaw.set_value(0); //leave goal in corner
    intakePiston.set_value(1);
    comboState = 3; //intake until ring of correct color detected
    toPointShortBy(24.0, 3.0, 0, 0, 5.0, 12000.0); //move to intake stacked top ring //!the 3 is actually to counteract what hte updating was meant to so its not sus promise mhm totally yeah thats right youre still reading bro thats crazy you literally need to go lock in RIGHT NOW oh my lanta why are you still here 😭 GET OUT
    comboState = 3;
    intakePiston.set_value(0); //intake stacked ring
    delay(250);
    comboState == 18;
    toPoint(22.0, -11.0, 0, 0, 0.0, 12000.0); //align for alliance stake scoring
    facePoint(xPos, -48.0); //face wall directly
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    delay(750);
    comboState = 0;
    intakePiston.set_value(0);
    toPointShortBy(0.0, 24.0, 1, 0, goalDist - 4.0, 12000.0); //reverse into second goal
    backClaw.set_value(1);
    delay(125);
    comboState = 1; //intake held ring onto second goal
    toPoint(-24.0, 24.0, 0, 1, 6.0, 12000.0); //intake ring into wall mech
    if(blueAWP != 1){
        comboState = 4;
        toPoint(-37.0, 37.0, 0, 0, 0.0, 12000.0); //go to wall stake
        facePoint(-52.0, 48.0); //face wall stake //too small of a movement i think
        comboState = 6; //score wall stake
        delay(750);
        //drivetrain.move_voltage(12000.0); //too likely to cross, just gotta bucket
        //delay(500);
        toPoint(0.0, 24.0, 1, 0, 0.0, 12000.0); //reverse to touch ladder
        toPoint(12.0, 36.0, 0, 0, 0.0, 12000.0); //touch ladder
    }
    else{
        comboState = 6;
        toPoint(-12.0, 36.0, 0, 0, 0.0, 12000.0); 
    }

}

void blueNegative(){ //!simply copied from red side and x coordinates negatived
    teamColor = COLOR_BLUE;
    comboState = 0;
    TPSB1 = 1; //don't update
    toPointShortBy(0.0, 24.0, 1, 0, goalDist, 12000.0); //align to first goal
    backClaw.set_value(1); //clamp first goal
    delay(125);
    intakePiston.set_value(1);
    comboState = 1;
    toPointShortBy(-24.0, -3.0, 0, 0, ringStackDist, 12000.0); //go to stacked ring //!no idea why it needs -3.0
    intakePiston.set_value(0); //intake stacked ring
    delay(125);
    comboState = 18;
    //intakePiston.set_value(1);
    toPoint(-18.0, -12.0, 0, 0, 0.0, 12000.0); //align for alliance wall stake
    facePoint(xPos, -48.0); //face alliance wall stake
    comboState = 6; //score on alliance stake
    delay(500);
    //drivetrain.move_voltage(6000.0);
    //delay(250);
    intakePiston.set_value(0);
    toPoint(xPos, 6.0, 1, 1, 6.0, 12000.0); //reverse away from alliance stake
    comboState = 1;
    leftClearer.set_value(1);
    toPoint(32.0, -12.0, 0, 1, 6.0, 12000.0); //go put clearer in corner
    toPoint(32.0, 0.0, 0, 1, 6.0, 12000.0); //push rings out
    leftClearer.set_value(0);
    facePoint(48.0, 24.0); //face rings
    toPoint(36.0, 27.0, 0, 1, 0.0, 6000.0); //intake all rings //pull +36 down //!4000.0
    toPoint(36.0, 18.0, 1, 1, 0.0, 12000.0); //back up from ring intake (if necessary?)
    if(blueAWP != 1){
        comboState = 4; //wall mech loading
        instantLift = 1;
        toPoint(24.0, 24.0, 0, 1, 6.0, 12000.0); //intake into wall mech
        toPoint(20.0, 38.5, 0, 1, 6.0, 12000.0); //go to ring pile
        delay(250);
        toPoint(24.0, 24.0, 1, 1, 6.0, 12000.0); //reverse from first line ring
        toPoint(26.0, 40.0, 0, 1, 6.0, 12000.0); //intake other ring
        delay(250);
        toPoint(24.0, 24.0, 1, 1, 6.0, 12000.0); //reverse outta there to align with wall stake
        facePoint(48.0, 48.0); //face wall stake
        toPoint(40.0, 46.0, 0, 0, 0.0, 12000.0); //score wall stake
        //toPointShortBy(-48.0, 50.0, 0, 0, wallStakeDist, 12000.0); //score wall stake
        toPoint(24.0, 24.0, 1, 1, 6.0, 12000.0); //away from wall stake
        toPoint(-12.0, 36.0, 0, 1, 6.0, 6000.0); //go touch ladder
    }
    else{
        toPoint(24.0, 24.0, 0, 1, 6.0, 12000.0); //intake bottom ring
        comboState = 6;
        toPoint(-12.0, 36.0, 0, 1, 0.0, 12000.0); //touch ladder
    }
}

void redPositive(){ // Line up with right side wallmech bearing flat front screw and back left screw in 7th hole
    
    /* //!this is jacob's code
    teamColor = COLOR_RED;
    TPSB1 = 0; //update
    comboState = 6; //wall mech fully extended, intake off
    delay(1000); // score alliance stake
    comboState = 0; //wall mwch idle intake off
    toPointShortBy(0.0,24.0,1,0,goalDist+8.5, 12000.0); // grab goal
    backClaw.set_value(1);
    delay(500);
    intakePiston.set_value(1); // lift up intake
    comboState = 1; //wall mech idle intake off
    toPointShortBy(-24.0,0.0,0,0,12.0, 12000.0); //ring stack
    delay(250);
    intakePiston.set_value(0);
    toPoint(0.0,0.0,1,1,12.0, 12000.0); //crank the 90
    toPoint(24.0,24.0,0, 1, 6.0, 12000.0); //bottom ring
    leftClearer.set_value(1);
    toPointShortBy(36.0, -10.0, 0, 0, 18.0, 12000.0); //put clearer in corner
    toPoint(-36.0, -14.0, 0, 0, 6.0, 12000.0); //clear corner
    facePoint(0.0, yPos); //align to rings
    toPoint(-12.0, -14.0, 0, 0, 6.0, 12000.0); //clear intake all corner rings
    leftClearer.set_value(0);
    toPoint(-24.0, 12.0, 0, 0, 1.0, 12000.0); //go to ladder
    facePoint(-24.0, 24.0); //face ladder
    drivetrain.move_voltage(2000.0); //ram ladder
    */

    //!this is carson's code
    teamColor = COLOR_RED;
    TPSB1 = 1; //don't update
    toPoint(xPos, 30.0, 1, 1, 0.0, 12000.0); //dodge ring
    faceAway(24.0, 30.0); //face goal backward //!36 //?try (24.0, 36.0) to arc into goal (might require increasing y val of previous call)
    toPointShortBy(24.0, 48.0, 1, 0, goalDist + 3.75, 5000.0); //goal rush
    backClaw.set_value(1);
    delay(250);
    comboState = 1; //intake, wall mech idle
    leftClearer.set_value(1);
    toPoint(36.0, -3.5, 0, 1, 0.0, 12000.0); //stick the corner clearer in
    facePoint(0.0, yPos); //clear rings
    toPoint(0.0, -12.0, 0, 1, 0.0, 6000.0); //intake all rings
    leftClearer.set_value(0);
    toPoint(36.0, -18.0, 1, 1, 6.0, 12000.0); //go back to corner for goal drop
    backClaw.set_value(0); //leave goal in corner
    intakePiston.set_value(1);
    comboState = 3; //intake until ring of correct color detected
    toPointShortBy(-24.0, 3.0, 0, 0, 5.0, 12000.0); //move to intake stacked top ring //!the 3 is actually to counteract what hte updating was meant to so its not sus promise mhm totally yeah thats right youre still reading bro thats crazy you literally need to go lock in RIGHT NOW oh my lanta why are you still here 😭 GET OUT
    comboState = 3;
    intakePiston.set_value(0); //intake stacked ring
    delay(250);
    comboState == 18;
    toPoint(-22.0, -11.0, 0, 0, 0.0, 12000.0); //align for alliance stake scoring
    facePoint(xPos, -48.0); //face wall directly
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    if(comboState == 10 || comboState == 1 || comboState == 0){comboState = 6;}
    comboState = 6; //wall mech fully extended, intake off
    delay(5);
    delay(750);
    comboState = 0;
    intakePiston.set_value(0);
    toPointShortBy(0.0, 24.0, 1, 0, goalDist - 4.0, 12000.0); //reverse into second goal
    backClaw.set_value(1);
    delay(125);
    comboState = 1; //intake held ring onto second goal
    toPoint(24.0, 24.0, 0, 1, 6.0, 12000.0); //intake ring into wall mech or onto goal depending on AWP

    if(redAWP != 1){
        comboState = 4;
        toPoint(37.0, 37.0, 0, 0, 0.0, 12000.0); //go to wall stake
        facePoint(52.0, 48.0); //face wall stake //too small of a movement i think
        comboState = 6; //score wall stake
        delay(750);
        //drivetrain.move_voltage(12000.0); //too likely to cross, just gotta bucket
        //delay(500);
        toPoint(0.0, 24.0, 1, 0, 0.0, 12000.0); //reverse to touch ladder
        toPoint(-12.0, 36.0, 0, 0, 0.0, 12000.0); //touch ladder
    }

    else{
        comboState = 6;
        toPoint(-12.0, 36.0, 0, 0, 0.0, 12000.0); 
    }
}

void redNegative(){
    teamColor = COLOR_RED;
    comboState = 0;
    TPSB1 = 1; //don't update
    toPointShortBy(0.0, 24.0, 1, 0, goalDist, 12000.0); //align to first goal
    backClaw.set_value(1); //clamp first goal
    delay(125);
    intakePiston.set_value(1);
    comboState = 1;
    toPointShortBy(24.0, -3.0, 0, 0, ringStackDist, 12000.0); //go to stacked ring //!no idea why it needs -3.0
    intakePiston.set_value(0); //intake stacked ring
    delay(125);
    comboState = 18;
    //intakePiston.set_value(1);
    toPoint(18.0, -12.0, 0, 0, 0.0, 12000.0); //align for alliance wall stake
    facePoint(xPos, -48.0); //face alliance wall stake
    comboState = 6; //score on alliance stake
    delay(500);
    //drivetrain.move_voltage(6000.0);
    //delay(250);
    intakePiston.set_value(0);
    toPoint(xPos, 6.0, 1, 1, 6.0, 12000.0); //reverse away from alliance stake
    comboState = 1;
    leftClearer.set_value(1);
    toPoint(-32.0, -12.0, 0, 1, 6.0, 12000.0); //go put clearer in corner
    toPoint(-32.0, 0.0, 0, 1, 6.0, 12000.0); //push rings out
    leftClearer.set_value(0);
    facePoint(-48.0, 24.0); //face rings
    toPoint(-36.0, 27.0, 0, 1, 0.0, 6000.0); //intake all rings //pull +36 down //!4000.0
    toPoint(-36.0, 18.0, 1, 1, 0.0, 12000.0); //back up from ring intake (if necessary?)

    if(redAWP != 1){
        comboState = 4; //wall mech loading
        instantLift = 1;
        toPoint(-24.0, 24.0, 0, 1, 6.0, 12000.0); //intake into wall mech
        toPoint(-20.0, 38.5, 0, 1, 6.0, 12000.0); //go to ring pile
        delay(250);
        toPoint(-24.0, 24.0, 1, 1, 6.0, 12000.0); //reverse from first line ring
        toPoint(-26.0, 40.0, 0, 1, 6.0, 12000.0); //intake other ring
        delay(250);
        toPoint(-24.0, 24.0, 1, 1, 6.0, 12000.0); //reverse outta there to align with wall stake
        facePoint(-48.0, 48.0); //face wall stake
        toPoint(-40.0, 46.0, 0, 0, 0.0, 12000.0); //score wall stake
        //toPointShortBy(-48.0, 50.0, 0, 0, wallStakeDist, 12000.0); //score wall stake
        toPoint(-24.0, 24.0, 1, 1, 6.0, 12000.0); //away from wall stake
        toPoint(12.0, 36.0, 0, 1, 6.0, 6000.0); //go touch ladder
    }
    else{
        toPoint(-24.0, 24.0, 0, 1, 6.0, 12000.0); //intake bottom ring
        comboState = 6;
        toPoint(12.0, 36.0, 0, 1, 0.0, 12000.0); //touch ladder
    }
}

void skills(){
    /*
    teamColor = COLOR_RED;
    comboState = 6; //score preload onto alliance stake
    toPointShortBy(-24.0, 0.0, 1, 0, goalDist); //go to first goal
    backClaw.set_value(1);
    comboState = 1;
    toPoint(-24.0, 24.0, 0, 1, 6.0); //first ring
    toPoint(-48.0, 72.0, 0, 1, 6.0); //second ring
    comboState = 4;
    toPoint(-48.0, 96.0, 0, 0, 0.0); //wall mech third ring
    facePoint(-48.0, 48.0); //about face
    toPoint(-48.0, 48.0, 0, 1, 6.0); //go to wall stake line
    comboState = 17;
    toPointShortBy(-72.0, 48.0, 0, 0, wallStakeDist); //align to wall stake
    comboState = 6; //score wall stake
    delay(500);
    comboState = 4; //intake second ring
    while(comboState != 5){delay(10);}
    comboState = 6; //score second wall stake ring
    delay(500);
    comboState = 1;
    toPoint(-48.0, 24.0, 0, 1, 6.0); //intake third goal ring
    toPoint(-48.0, -12.0, 0, 1, 6.0); //intake fourth and fifth goal rings
    toPoint(-60.0, 0.0, 0, 1, 6.0); //intake sixth goal ring
    toPointShortBy(-72.0, -24.0, 1, 0, cornerDist); //push into corner
    backClaw.set_value(0);
    toPointShortBy(24.0, 0.0, 1, 0, goalDist); //go to second goal
    backClaw.set_value(1);
    toPoint(24.0, 24.0, 0, 1, 6.0); //intake first ring
    toPoint(0.0, 48.0, 0, 1, 6.0); //intake second ring
    toPoint(24.0, 72.0, 0, 1, 6.0); //go to third ring and wall mech it
    comboState = 4;
    toPoint(48.0, 48.0, 0, 1, 6.0); //align onto wall stake line
    comboState = 17;
    comboState = 6;
    toPointShortBy(72.0, 48.0, 0, 0, wallStakeDist); //score wall stake
    comboState = 4;
    while(comboState != 5){delay(10);}
    comboState = 6; //score second wall stake ring
    delay(500);
    comboState = 1;
    toPoint(48.0, 24.0, 0, 1, 6.0); //intake third goal ring
    toPoint(48.0, -12.0, 0, 1, 6.0); //intake fourth and fifth goal rings
    toPoint(60.0, 0.0, 0, 1, 6.0); //intake sixth goal ring
    toPointShortBy(72.0, -24.0, 1, 0, cornerDist); //goal in corner
    backClaw.set_value(0);
    comboState = 3;
    toPoint(48.0, 72.0, 0, 1); //intake field ring and pause on intake
    toPointShortBy(0.0, 96.0, 1, 0, goalDist); //grab far center goal
    backClaw.set_value(1);
    comboState = 1;
    facePoint(24.0, 96.0); //face aligned rings
    toPoint(24.0, 72.0, 0, 1); //first bottom ring
    toPoint(36.0, 72.0, 0, 1); //second bottom ring
    toPoint(24.0, 108.0, 0, 1); //third bottom ring
    toPoint(-24.0, 72.0, 0, 1); //fifth goal ring
    toPoint(60.0, 96.0, 0, 1); //sixth goal ring
    faceAway(-72.0, 120.0); //point away from corner
    comboState = 0;
    toPointShortBy(-72.0, 120.0, 1, 0, cornerDist); //put goal in corner 
    toPointShortBy(-24.0, 108.0, 1, 1, goalDist); //go to blue ring goal
    backClaw.set_value(1);
    toPointShortBy(72.0, 120.0, 1, 0, cornerDist); //final corner
    comboState = 6;
    toPoint(12.0, 60.0, 1, 1); //hang
    */
}
    