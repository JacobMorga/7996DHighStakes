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

void skills(){
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
    toPointShortBy(-24.0, 0.0, 1, 1, goalDist, 12000.0); //reverse into first goal
    backClaw.set_value(1); //grab first goal
    purePursuit(sPath1, 24.0, 12000.0, 0); //intake three rings
    transit(4); //put last ring into wall mech
    toPoint(-36.0, 48.0, 1, 1, 12000.0); //reverse back to wall stake
    transit(17); //only intake on bottom
    toPoint(-60.0, 48.0, 0, 0, 6000.0); //score wall stake
    transit(4);
    while(comboState != 7){delay(10);}
    delay(500);
    transit(1);
    toPoint(-36.0, 48.0, 1, 1, 12000.0);
    purePursuit(sPath2, 24.0, 12000.0, 1);
    toPoint(-60.0, 0.0, 0, 1, 12000.0);
    toPoint(-66.0, -18.0, 1, 1, 12000.0);
    backClaw.set_value(0);
    toPointShortBy(24.0, 0.0, 1, 0, goalDist, 12000.0);
    backClaw.set_value(1);  
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