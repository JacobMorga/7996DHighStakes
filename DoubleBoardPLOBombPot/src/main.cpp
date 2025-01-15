#include "main.h"
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {

	drive1.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive2.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive3.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive4.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive5.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive6.set_brake_mode(MOTOR_BRAKE_BRAKE);
	
	delay(200);
	screen::erase();
	screen::set_eraser(COLOR_BLACK);
	delay(200);

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


	inertial1.reset();
	inertial2.reset();
	inertial3.reset();
	yTracking.set_reversed(1);
	xTracking.set_reversed(1);
	yTracking.reset();
	xTracking.reset();

	while(inertial1.is_calibrating() || inertial2.is_calibrating() || inertial3.is_calibrating()){
		delay(20);
	}	

	delay(3000);

	pros::lcd::initialize();

	pros::Task odomTask (odometry, "odomTask");

	screen::erase();

	//!THIS TASK MUST BE COMMENTED TO RUN OR TEST COLOR CALIBRATION.
	//!MAKE SURE IT IS UNCOMMENTED AFTER COLOR CALIBRATION.
	Task intakeTask (runIntake, "intakeTask");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	autonSelector();
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

	screen::erase(); // Erases auton selector

	screen::set_pen(teamColor);
	screen::fill_rect(0,0,480,240);
	screen::set_pen(COLOR_BLACK);
	screen::set_eraser(teamColor);
	
	if      (autonSelected == 1){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Red Pos"); redPositive();} // Runs each auton based on 
	else if (autonSelected == 2){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Red Neg"); redNegative();}
	else if (autonSelected == 3){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Blue Pos"); bluePositive();}
	else if (autonSelected == 4){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Blue Neg"); blueNegative();}
	else if (autonSelected == 5){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"); skills();}
	else if (autonSelected == 6){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"); skills();}
	else if (autonSelected == 7){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"); skills();}
	else if (autonSelected == 8){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"); skills();}
	else if (autonSelected == 9){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Skills"); skills();}
	else { printAtPoint(TEXT_LARGE_CENTER, 100, 100, "YOU'RE COOKED"); }


}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {

	runDriveCont();
	//ploinker.set_value(1);
	//calculateOffsets();
	//lcd::set_text(7, "we're in driver now yo");
	//toPoint(48.0, 48.0, 0.0, false);
	//lcd::set_text(7, "freaky time");
	//teamColor = COLOR_RED;
	//redPositive();
	//redNegative();
	//skills();

	//toPoint(0.0, 48.0, 0, 0);
	//toPoint(0.0, 0.0, 1, 0);

	//!TO RUN OR TEST COLOR CALIBRATION THE INTAKE TASK IN THIS FILE MUST BE COMMENTED.
	//!MAKE SURE TO RE-ENABLE IT AFTER COLOR CALIBRATION.
	//runColorCalibration();
	//testColorCalibration();
}
