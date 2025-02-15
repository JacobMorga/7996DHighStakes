#include "main.h"

using namespace pros;
float goalDist = 6.0;
float cornerDist = 18.0;
float ringStackDist = 9.0;
float clearingDist = 12.0;
float wallStakeDist = 10.0;

void bluePositive(){
    teamColor = COLOR_BLUE;
}

void blueNegative(){
    teamColor = COLOR_BLUE;
}

void redPositive(){ // Line up with right side wallmech bearing flat front screw and back left screw in 7th hole
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


    /*
    teamColor = COLOR_RED;
    toPoint(12, 36.0, 1, 1, 12.0); //dodge ring
    //toPoint(24.0, 36.0, 1, 1); //get in between ring and goal
    toPointShortBy(24.0, 48.0, 1, 0, goalDist + 15.5); //goal rush
    //toPoint(30.0,39.0,1,0,0.0);
    backClaw.set_value(1);
    delay(500);
    
    comboState = 1; //intake, wall mech idle
    leftClearer.set_value(1);
    toPointShortBy(40.0, -22.0, 0, 0, 16.0); //corner clearer behind rings
    toPoint(24.0, -18.0, 0, 0, 0.0); //clear corner and intake all rings
    delay(2000); //make sure all were intaken
    toPoint(36.0, -18.0, 1, 1, 6.0); //go back to corner for goal drop
    backClaw.set_value(0); //leave goal in corner
    leftClearer.set_value(0);
    intakePiston.set_value(1);
    comboState = 3; //intake until ring of correct color detected
    toPointShortBy(-24.0, 0.0, 0, 0, ringStackDist); //move to intake stacked top ring
    comboState = 3;
    intakePiston.set_value(0); //intake stacked ring
    delay(250);
    intakePiston.set_value(1);
    toPoint(-21.0, -12.0, 0, 0, 0.0); //align for alliance stake scoring
    facePoint(xPos, -48.0); //face wall directly
    comboState = 6; //wall mech fully extended, intake off
    delay(2500);
    comboState = 0;
    intakePiston.set_value(0);
    toPointShortBy(0.0, 24.0, 1, 0, goalDist); //reverse into second goal
    backClaw.set_value(1);
    delay(500);
    comboState = 1; //intake held ring onto second goal
    toPoint(24.0, 24.0, 0, 1, 6.0); //intake ring into wall mech
    comboState = 4;
    toPointShortBy(48.0, 48.0, 0, 0, wallStakeDist); //score wall stake
    comboState = 6; //score wall stake
    delay(2500);
    toPoint(-12.0, 24.0, 0, 0, 0.0); //go to touch ladder
    drivetrain.move_voltage(6000.0); //touch ladder
    */
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
    