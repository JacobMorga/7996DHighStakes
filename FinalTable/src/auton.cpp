#include "main.h"
using namespace pros;
float goalDist = 8.0;
float ringStackDist = 4.0;
bool runThisCodeShort = 0;

void bluePosAWP(){
    teamColor = COLOR_BLUE;
    if (runThisCodeShort == false){

    }
}

void blueNegAWP(){ 
    teamColor = COLOR_BLUE;
    specialIntake = 0;

    std::vector<coord> bnaPath1 = {coord(36.0, 38.0), coord(0.0, 38.0), coord(0.0, 24.0), coord(-24.0, 0.0)};

    transit(7); //alliance stake
    delay(500);
    transit(0);
    toPoint(1.0, 27.0, 1, 1, 7000.0); //grab goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    toPoint(36.0, 24.0, 0, 0, 12000.0); //first ring stack all the way in
    delay(500);
    toPoint(36.0, 36.0, 0, 1, 12000.0); //align ring cube
    purePursuit(bnaPath1, 24.0, 6000.0, 0); //get ring cube and go to top blue ring
    if (runThisCodeShort == false){transit(3);}
    toPoint(-30.0,-6.0,0,0,12000);
    delay(750);
    toPoint(-24.0, -6.0, 1, 1, 12000.0); //put goal behind

    if (runThisCodeShort == false){
        backClaw.set_value(0);
        toPoint(-24.0, 6.0, 0, 1, 12000.0); //away from goal
        toPoint(-51.0, 27.0, 1, 1, 12000.0); //second goal
        backClaw.set_value(1);
        delay(100);
        transit(1); //load held ring
        delay(1000);
        toPoint(-40.0, 32.0, 0, 0, 12000.0); //touch ladder (try backward?)
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

    toPoint(36.0, 30.0, 0, 1, 12000.0);
    faceHeading(145.0, 1, 12000.0);
    rightClearer.set_value(1);
    delay(500);
    smoothFPError = 10.0;
    faceHeading(90.0, 0, 8000.0);
    toPoint(36.0, 0.0, 1, 0, 12000.0);
    faceHeading(135.0, 1, 12000.0);
    rightClearer.set_value(0);
    delay(100);
    faceHeading(110.0, 1, 12000.0); //let arm come up
    faceHeading(-90.1, 0, 12000.0); //face away from goal counter clockwise
    toPoint(xPos, 18.0, 1, 1, 12000.0);
    backClaw.set_value(1);
    delay(250);
    transit(1);
    toPoint(22.0, 26.0, 0, 1, 12000.0); //intake bottom red ring
    delay(500); //possession
    purePursuit({coord(24.0, 24.0), coord(12.0, 12.0), coord(-18.0, 0.0)}, 12.0, 12000.0, 1);
    toPoint(-24.0, 0, 0, 1, 3000.0); //intake bottom blue ring slowly
    delay(500); //possession
    toPoint(-36.0, -3.0, 0, 1, 12000.0); //intake top red ring
    toPoint(-23.0, -9.5, 0, 1, 12000.0); //align for wall stake
    transit(0);
    faceHeading(-90.0, 0, 12000.0); //face wall stake //try slower if necessary
    transit(6);
    delay(750); //scoring
    if(runThisCodeShort == false){ //awp
        transit(2);
        toPoint(-24.0, 24.0, 0, 0, 6000.0); //touch ladder
    }

    else if (runThisCodeShort == true){ //max
        transit(1);
        toPoint(36.0, -18.0, 0, 0, 12000.0);
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

    transit(7); //alliance stake
    delay(500);
    transit(0);
    toPoint(-3.0, 27.0, 1, 1, 7000.0); //grab goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    toPoint(-36.0, 24.0, 0, 0, 12000.0); //first ring stack all the way in
    delay(500);
    toPoint(-36.0, 36.0, 0, 1, 12000.0); //align ring cube
    purePursuit(rnaPath3, 24.0, 6000.0, 0); //get ring cube and go to top red ring
    if (runThisCodeShort == false){transit(3);}
    toPoint(30.0,-6.0,0,0,12000);
    delay(750);
    toPoint(24.0, -6.0, 1, 1, 12000.0); //put goal behind

    if (runThisCodeShort == false){
        backClaw.set_value(0);
        toPoint(24.0, 6.0, 0, 1, 12000.0); //away from goal
        toPoint(51.0, 27.0, 1, 1, 12000.0); //second goal
        backClaw.set_value(1);
        delay(100);
        transit(1); //load held ring
        delay(1000);
        toPoint(40.0, 32.0, 0, 0, 12000.0); //touch ladder (try backward?)
    }
}

void bluePosMax(){
    teamColor = COLOR_BLUE;
}

void blueNegMax(){ 
    teamColor = COLOR_BLUE;
}

void redPosMax(){
    teamColor = COLOR_RED;
}

void redNegMax(){
    teamColor = COLOR_RED;
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
    std::vector<coord> sPathE = {coord(-48.0,72.0),coord(-24.0,48.0),coord(-96.0, 54.0)};
    std::vector<coord> sPathF = {coord(-24.0, 24.0), coord(48.0, 96.0)};
    std::vector<coord> sPathG = {coord(64.0, -14.0), coord(38.0, 66.0), coord(72.0, 40.0)};
    std::vector<coord> sPathH = {coord(60.0, 48.0), coord(32.0, 80.0), coord(-22.0, 110.0), coord(-60.0, 116.0)};
    bezierCurve(coord(-14.0, -11.0), coord(-48.0, 25.0), coord(15.0, 37.0), coord(93.0, 152.0), coord(61.0, 96.0), 100, sPathA);
    bezierCurve(coord(0.0, 96.0), coord(-48.0, 44.0), coord(-48.0, 200.0), coord(-116.0, 80.0), coord(-48.0, 96.0), 100, sPathB);
    bezierCurve(coord(-48.0, 72.0), coord(-24.0, 48.0), coord(-24.0, 6.0), coord(-60.0, 50.0), coord(-86.0, 45.0), 100, sPathC);
    bezierCurve(coord(24.0, 0.0), coord(-36.0, -9.0), coord(66.0, 66.0), coord(48.0, 51.0), coord(48.0, -18.0), 100, sPathD);

    transit(6); //score alliance stake ring
    delay(500);
    transit(1);
    delay(500);
    //toPointShortBy(-24.0, 0.0, 1, 1, goalDist - 6.0, 12000.0); //get first goal but go extra far to start the pure pursuit at the right spot
    toPoint(-27.0, 3.0, 1, 1, 12000.0); //!-3x +3y
    backClaw.set_value(1);
    delay(100);
    //whatToRun = 1;
    purePursuit(sPathF, 36.0, 12000.0, 0); //through ladder //!24 //!sPathA for bezier
    //intakePiston.set_value(0);
    //intakePiston.set_value(1);
    toPoint(57.0, 96.0, 0, 0, 8000.0); //right wall ring
    delay(500);
    //toPoint(48.0, 96.0, 0, 1, 12000.0); //middle ring
    toPoint(42.0, 90.0, 1, 1, 12000.0); //more space to get far ring
    toPoint(48.0, 112.0, 0, 1, 8000.0); //far ring
    //delay(500); //just get that last ring
    toPoint(63.0, 112.0, 0, 0, 12000.0); //clear corner by intaking
    toPoint(48.0, 96.0, 1, 1, 12000.0); //back for second clear
    facePoint(72.0, 108.0, 0, 12000.0);
    toPoint(69.0, 108.0, 0, 1, 12000.0); //clear corner by intaking //!63, 112
    toPoint(69.0, 112.0, 0, 0, 12000.0); //farther in clearing
    //delay(500); //let the color sort get the ring out
    toPoint(48.0, 96.0, 1, 1, 12000.0); //away from corner
    faceAway(72.0, 120.0, 0, 12000.0); //ready to corner the goal //!144 cause it was nacking earlier or sm idk

    //bad post bezier corner
    //toPoint(42.0, 90.0, 1, 1, 12000.0); //reverse to get sixth ring
    //toPoint(48.0, 108.0, 0, 1, 12000.0); //get sixth ring
    //toPoint(66.0, 114.0, 0, 0, 12000.0); //clear corner by intaking
    //delay(500);
    //toPoint(54.0, 114.0, 1, 1, 12000.0); //clear corner by intaking
    //toPoint(66.0, 114.0, 0, 0, 12000.0); //clear corner by intaking
    //delay(500);
    //faceAway(72.0, 0, 120.0); //turn to put goal in corner
    //toPoint(60.0, 108.0, 1, 0, 12000.0); //put goal in corner
    //

    backClaw.set_value(0); //let go early so only the goal tilts and not the robot or the goal doesnt tilt at all
    toPoint(60.0, 114.0, 1, 1, 12000.0); //goal in corner //!+6y
    toPoint(18.0, 90.0, 0, 1, 12000.0); //forward and maybe move some rings that got in the way
    toPoint(0.0, 96.0, 1, 1, 12000.0); //go to back middle goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    //purePursuit(sPathB, 24.0, 12000.0, 0); //corner three rings
    
    //instead of pure pursuit (too small of movements in the corner)
    toPoint(-24.0, 90.0, 0, 1, 12000.0); //between ring and goal
    rightClearer.set_value(1);
    toPoint(-57.0, 114.0, 0, 1, 12000.0); //back corner
    facePoint(xPos+0.01, 0.0, 0, 8000.0); //no idea if xPos on its own would funch arctan2 (shouldnt) but whatever
    rightClearer.set_value(0); //yes i know its kinda late here but let me cook nope its back up here bro did not cook
    toPoint(-54.0, 84.0, 0, 1, 12000.0); //second bottom ring
    toPoint(-48.0, 90.0, 0, 1, 12000.0); //last bottom ring //!-6y
    //

    toPoint(-18.0, 66.0, 0, 0, 6000.0); //one more onto the goal
    toPoint(-48.0, 72.0, 0, 1, 6000.0); //wall mech this one
    transit(4);
    specialIntake = 1; //not sure where this goes
    purePursuit(sPathE, 24.0, 12000.0, 0); //go to first wall stake //! was path C but it hit the tower
    transit(17);
    //toPoint(-42.0, 48.0, 1, 1, 12000.0); //back up
    toPoint(-54.0, 48.0, 1, 0, 12000.0); //to wall stake
    facePoint(-120.0, yPos, 0, 12000.0);
    transit(19); //mech up
    delay(500);
    toPoint(-60.0, 48.0, 0, 0, 12000.0); //ring on stake push
    delay(1000);
    instantLift = 1;
    transit(4); //load second ring into mech and lift
    toPoint(-54.0, 48.0, 1, 0, 12000.0);
    facePoint(-120.0, yPos, 0, 12000.0);
    delay(1000);
    toPoint(-60.0, 48.0, 0, 0, 12000.0); //push ring onto stake

    //toPoint(-60.0, 48.0, 0, 1, 12000.0); //intake second wall stake ring
    //toPoint(-42.0, 48.0, 1, 0, 12000.0); //back up
    //delay(1000);
    //toPoint(-63.0, 48.0, 0, 1, 12000.0); //score second ring
    toPoint(-42.0, 48.0, 1, 0, 12000.0); //back up to align with ring line
    instantLift = 0;
    transit(1);
    toPoint(-48.0, -18.0, 0, 1, 8000.0); //line of 3 rings
    toPoint(-66.0, 6.0, 0, 1, 12000.0); //last ring but its just in the bottom of the intake
    transit(3); //? is this too late
    toPoint(-60.0, -12.0, 1, 1, 12000.0); //goal in corner
    backClaw.set_value(0); //early you doofus no it nacked bruh pout it back
    toPoint(0.0, 6.0, 0, 1, 12000.0); //toward other goal forward so we hold the intake ring
    toPoint(24.0, 0.0, 1, 1, 12000.0); //get other goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    specialIntake = 0; //think this needs to go off (its probably funching the wallmech)
    purePursuit(sPathD, 18.0, 8000.0, 0); //finish bottom right corner goal //!24 is probably path skippingly big
    toPoint(60.0, 0.0, 0, 1, 12000.0); //wall mech ring path
    transit(4); 
    toPoint(60.0, 12.0, 0, 1, 12000.0); //wall mech ring all the way
    backClaw.set_value(0); //early, remember bruh
    toPoint(60.0, -12.0, 1, 1, 12000.0); //score goal
    transit(17);
    purePursuit(sPathG, 24.0, 12000.0, 0); //wall stake 
    toPoint(54.0, 48.0, 1, 1, 12000.0); //back up with second ring in bottom of intake
    facePoint(120.0, yPos, 0, 12000.0);
    transit(19);
    delay(500);
    drivetrain.move_voltage(8000.0);
    delay(1000);
    toPoint(54.0, 48.0, 1, 1, 12000.0); //back up
    transit(4);
    delay(1000); //let ring in?
    transit(6);
    delay(500);
    drivetrain.move_voltage(8000.0);
    delay(1000); //score second wall stake ring
    //toPoint(60.0, 48.0, 0, 1, 12000.0);
    transit(1);
    delay(50);
    //toPoint(48.0, 48.0, 1, 1, 12000.0); //space away from wall stake to not get caught
    purePursuit(sPathH, 24.0, 12000.0, 0); //last corner
    transit(6);
    toPoint(0.0, 48.0, 1, 0, 8000.0); //hang without executing itself
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

    toPoint(36.0, 33.0, 0, 1, 12000.0);
    faceHeading(150.0, 1, 12000.0);
    rightClearer.set_value(1);
    delay(100);
    faceHeading(90.0, 1, 12000.0);
    toPoint(36.0, 0.0, 1, 0, 12000.0);
}