#include "main.h"
using namespace pros;
float goalDist = 8.0;

void bluePosAWP(){

}

void blueNegAWP(){ 

}

void redPosAWP(){

}

void redNegAWP(){

}

void bluePosMax(){

}

void blueNegMax(){ 

}

void redPosMax(){

}

void redNegMax(){

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
    teamColor = COLOR_RED;
    instantLift = 1;
    specialIntake = 1;
    std::vector<coord> sPathA = {};
    std::vector<coord> sPathB = {};
    std::vector<coord> sPathC = {};
    std::vector<coord> sPathD = {};
    std::vector<coord> sPathE = {};
    std::vector<coord> sPathF = {};
    std::vector<coord> sPathG = {};
    std::vector<coord> sPathH = {};
    bezierCurve(coord(-14.0, -11.0), coord(-48.0, 25.0), coord(15.0, 37.0), coord(93.0, 152.0), coord(61.0, 96.0), 100, sPathA);
    bezierCurve(coord(0.0, 96.0), coord(-48.0, 44.0), coord(-48.0, 200.0), coord(-116.0, 80.0), coord(-48.0, 96.0), 100, sPathB);
    bezierCurve(coord(-48.0, 72.0), coord(-24.0, 48.0), coord(-24.0, 6.0), coord(-48.0, 60.0), coord(-66.0, 60.0), 100, sPathC);
    bezierCurve(coord(24.0, 0.0), coord(18.0, 54.0), coord(40.0, 80.0), coord(53.0, 61.0), coord(48.0, -12.0), 100, sPathD);

    transit(6); //score alliance stake ring
    delay(750);
    transit(1);
    delay(500);
    //toPointShortBy(-24.0, 0.0, 1, 1, goalDist - 6.0, 12000.0); //get first goal but go extra far to start the pure pursuit at the right spot
    toPoint(-24.0, 0.0, 1, 1, 12000.0);
    backClaw.set_value(1);
    delay(100);
    purePursuit(sPathA, 24.0, 12000.0, 0); //through ladder
    toPoint(42.0, 90.0, 1, 1, 12000.0); //reverse to get sixth ring
    toPoint(48.0, 108.0, 0, 1, 12000.0); //get sixth ring
    toPoint(66.0, 114.0, 0, 0, 12000.0); //clear corner by intaking
    delay(1000);
    faceAway(72.0, 120.0); //turn to put goal in corner
    toPoint(60.0, 108.0, 1, 0, 12000.0); //put goal in corner
    backClaw.set_value(0);
    toPoint(48.0, 86.0, 0, 1, 12000.0); //forward a bit
    toPoint(0.0, 96.0, 1, 1, 12000.0); //go to back middle goal
    backClaw.set_value(1);
    delay(100);
    //purePursuit(sPathB, 24.0, 12000.0, 0); //corner three rings
    
    //instead of pure pursuit (too small of movements in the corner)
    toPoint(-24.0, 90.0, 0, 1, 12000.0); //between ring and goal
    toPoint(-48.0, 108.0, 0, 1, 12000.0); //back corner
    toPoint(-60.0, 96.0, 0, 1, 12000.0); //second bottom ring
    toPoint(-48.0, 96.0, 0, 1, 12000.0); //last bottom ring
    //

    toPoint(-24.0, 72.0, 0, 1, 12000.0); //one more onto the goal
    toPoint(-48.0, 72.0, 0, 1, 12000.0); //wall mech this one
    purePursuit(sPathC, 24.0, 12000.0, 0); //score first wall stake
    toPoint(-48.0, 48.0, 1, 1, 12000.0); //back up
    transit(4);
    toPoint(-63.0, 48.0, 0, 1, 12000.0); //score second ring
    toPoint(-48.0, 48.0, 1, 1, 12000.0); //back up again
    toPoint(-48.0, -12.0, 0, 1, 12000.0); //line of 3 rings
    transit(3);
    toPoint(-60.0, 0.0, 0, 1, 12000.0); //last ring but its just in the bottom of the intake
    toPoint(-60.0, -12.0, 1, 1, 12000.0); //goal in corner
    backClaw.set_value(0);
    toPoint(6.0, 6.0, 0, 1, 12000.0); //toward other goal forward so we hold the intake ring
    toPointShortBy(24.0, 0.0, 1, 1, goalDist, 12000.0); //get other goal
    backClaw.set_value(1);
    delay(100);
    transit(1);
    purePursuit(sPathD, 18.0, 12000.0, 0); //finish bottom right corner goal //!24 is probably path skippingly big
    toPoint(60.0, 0.0, 1, 0, 12000.0); //wall mech ring
    transit(4);
    toPoint(60.0, -12.0, 1, 1, 12000.0); //score goal
}

void loadPaths(){

}

void testing(){
	//backClaw.set_value(1);
	//delay(500);
	//transit(1);
	//colorSorting = 0;

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
}