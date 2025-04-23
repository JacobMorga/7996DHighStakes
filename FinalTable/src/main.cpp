#include "main.h"
using namespace pros;

void initialize() {
	screen::erase();
	screen::set_eraser(COLOR_BLACK);
	screen::set_pen(COLOR_YELLOW);
	screen::fill_rect(0, 0, 480, 240);
	screen::set_eraser(COLOR_YELLOW);
	screen::set_pen(COLOR_BLACK);
	screen::print(TEXT_MEDIUM_CENTER, 120, 118, "Calibrating Inertial & Odom");

	drive1.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive2.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive3.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive4.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive5.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive6.set_brake_mode(MOTOR_BRAKE_BRAKE);
	intakeTop.set_brake_mode(MOTOR_BRAKE_BRAKE);
	intakeBottom.set_brake_mode(MOTOR_BRAKE_COAST);
	wallMech.set_brake_mode(MOTOR_BRAKE_HOLD); //don't think it's ever braking but whatever
	
    inertial1.reset();
    inertial2.reset();
    inertial3.reset();
	xTracking.reset();
	yTracking.reset();
    xTracking.set_position(0.0);
    yTracking.set_position(0.0);
	xTracking.set_reversed(0);
	yTracking.set_reversed(1);
	
	//while(inertial1.is_calibrating() || inertial2.is_calibrating() || inertial3.is_calibrating()){delay(20);}
	//delay(500);

	inertial1.set_rotation(0.0);
	inertial2.set_rotation(0.0);
	inertial3.set_rotation(0.0);
	delay(500.0);
	
	pros::lcd::initialize();
	pros::screen::erase();

	Task odomTask (odometry, "odomTask");
	Task comboTask (runComboSystem, "comboTask");
	Task WMTask (runWallMech, "WMTask");
	Task RCDCTask(runCodeDuringCode, "RCDCTask");
}

void disabled() {
	//FUNCH(me);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field Management System 
 * This task will exit when the robot is enabled and autonomous or opcontrol starts.
 */
void competition_initialize() {
	autonSelector();
}


void autonomous() {
	screen::erase(); // Erases auton selector

	screen::set_pen(teamColor); // Colors brain screen with team color
	screen::fill_rect(0,0,480,240);
	screen::set_pen(COLOR_BLACK);
	screen::set_eraser(teamColor);
	runThisCodeShort = 0;
	
	if      (autonSelected == 1 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BP A L");  bluePosAWP();} // Runs auton based on auton selector output
	else if (autonSelected == 2 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BN A L"); 	blueNegAWP();} // And prints what auton its running
	else if (autonSelected == 3 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RP A L"); 	redPosAWP();}
	else if (autonSelected == 4 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RN A L"); 	redNegAWP();}
	else if (autonSelected == 5 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BP Max");  bluePosMax();}
	else if (autonSelected == 6 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BN Max");  blueNegMax();}
	else if (autonSelected == 7 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RP Max");  redPosMax();}
	else if (autonSelected == 8 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RN Max"); 	redNegMax();}
	else if (autonSelected == 9 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BP A S");	runThisCodeShort = true; bluePosAWP();}
	else if (autonSelected == 10){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BN A S");	runThisCodeShort = true; blueNegAWP();}
	else if (autonSelected == 11){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RP A S");	runThisCodeShort = true; redPosAWP();}
	else if (autonSelected == 12){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RN A S");	runThisCodeShort = true; redNegAWP();}
	else if (autonSelected == 13){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills");	skills2();}
	else if (autonSelected == 14){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills");	skills2();}
	else if (autonSelected == 15){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Driver S"); }
	else if (autonSelected == 16){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Testing");	autonTesting();}
	
	else {printAtPoint(TEXT_LARGE_CENTER, 100, 100, "YOU'RE COOKED");}
}

void opcontrol() {

	//drivetrain.set_brake_modes(MOTOR_BRAKE_COAST);
	//drivetrain.brake();

	
	if(driverSkills){
		transit(6);
		delay(500);
		transit(1);
		backClawBool = 0;
	}
	runDriveCont();
	



	//toPointShortBy(48.0, -24.0, 0, 1, clearingDist);
	//calculateOffsets();
	//runColorCalibration();
	//testColorCalibration();
	//facePoint(24.0, 0.0);

	//toPoint(0.0, 72.0, 0, 0);
	//toPoint(96.0, 24.0, 0, 0);

	//toPoint(24.0, -72.0, 0, 0);
	//toPoint(0.0, 0.0, 0, 0);
	
	//toPoint(0.0, 48.0, 0, 0);
	//toPoint(0.0, 0.0, 1, 0);
	//toPoint(0.0, 48.0, 1, 0);
	//toPoint(0.0, 0.0, 0, 0);
	
	//toPoint(48.0, 48.0, 0, 0);
	//toPoint(96.0, 0.0, 0, 0);
	//toPoint(48.0, -48.0, 0, 0);
	//toPoint(0.0, 0.0, 0, 0);

	
	//std::vector<coord> pathA = {coord(0.0, 0.0), coord(0.0, 96.0), coord(-96.0, 0.0), coord(-96.0, 96.0)};

	/*
	rightDrive.set_brake_modes(MOTOR_BRAKE_COAST);
	leftDrive.set_brake_modes(MOTOR_BRAKE_COAST);
	rightDrive.brake();
	leftDrive.brake();
	*/

}