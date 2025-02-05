#include "main.h"

void initialize() {
	screen::erase();
	screen::set_eraser(COLOR_BLACK);
	screen::set_pen(COLOR_YELLOW);
	screen::fill_rect(0,0,480,240);
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
	wallMech.set_brake_mode(MOTOR_BRAKE_HOLD); //dont think its ever braking but whatever
	
	inertial1.reset();
	inertial2.reset();
	inertial3.reset();
	yTracking.set_reversed(1);
	xTracking.set_reversed(0);
	yTracking.reset();
	xTracking.reset();
	wallMech.tare_position();
	
	//while(inertial1.is_calibrating() || inertial2.is_calibrating() || inertial3.is_calibrating()){delay(20);}
	delay(500);
	
	pros::lcd::initialize();
	pros::screen::erase();

	Task odomTask (odometry, "odomTask");
	Task intakeAndWallMechTask (runIntakeAndWallMech, "intakeAndWallMechTask");
	Task button (USETHEBUTTONS, "buttonstask");
}

void disabled() {}

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
	
	if      (autonSelected == 1){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Red Pos" );	redPositive();} // Runs auton based on auton selector output
	else if (autonSelected == 2){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Red Neg" );	redNegative();} // And prints what auton its running
	else if (autonSelected == 3){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Blue Pos");	bluePositive();}
	else if (autonSelected == 4){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Blue Neg");	blueNegative();}
	else if (autonSelected == 5){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"  );	skills();}
	else if (autonSelected == 6){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"  );	skills();}
	else if (autonSelected == 7){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"  );	skills();}
	else if (autonSelected == 8){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"  );	skills();}
	else if (autonSelected == 9){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"  );	skills();}
	else {printAtPoint(TEXT_LARGE_CENTER, 100, 100, "YOU'RE COOKED");}
	
}

void opcontrol() {
	//runDriveCont();
	//calculateOffsets();
	/*
	while(1){
		lcd::set_text(1, std::to_string(getAngle()));
		delay(10);
	}
	*/

	while(1){
		std::cout << getR1() << getR2() << getL1() << getL2() << getA() << getB() << getX() << getY() << getUP() << getDOWN() << getRIGHT() << getLEFT() << "\n";
		delay(5);
	}
} 