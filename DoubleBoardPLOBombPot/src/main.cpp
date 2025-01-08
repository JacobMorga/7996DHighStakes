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
	/*
	delay(200);
	screen::erase();
	screen::set_eraser(COLOR_BLACK);
	delay(200);

	screen::set_pen(COLOR_YELLOW);
	screen::fill_rect(0,0,480,240);
	screen::set_eraser(COLOR_YELLOW);
	screen::set_pen(COLOR_BLACK);
	screen::print(TEXT_MEDIUM_CENTER, 120, 118, "Calibrating Intertal & Odom");
	
	drive1.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive2.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive3.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive4.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive5.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive6.set_brake_mode(MOTOR_BRAKE_BRAKE);

	intake.set_brake_mode(MOTOR_BRAKE_COAST);
	
	inertial1.reset();
	inertial2.reset();
	inertial3.reset();
	yTracking.reset();
	xTracking.reset();

	while(inertial1.is_calibrating() || inertial2.is_calibrating() || inertial3.is_calibrating()){

		delay(20);
	}
	delay(250);
	

	delay(3000);

	pros::Task odomTask (odometry, "odomTask");

	delay(250);

	screen::erase();
	*/

	pros::lcd::initialize();

	Task intakeTask (runIntake, "intakeTask");
	//Task intakeTask (firstTryColorSorting, "intakeTask");
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
	delay(2500);
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!autonSelector();
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
	/*
	//!this was maybe important idrk
	screen::erase(); // Erases auton selector

	screen::set_pen(teamColor);
	screen::fill_rect(0,0,480,240);
	screen::set_pen(COLOR_BLACK);
	screen::set_eraser(teamColor);
	

	if      (autonSelected == 1){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton1"); } // Runs each auton based on 
	else if (autonSelected == 2){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton2"); }
	else if (autonSelected == 3){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton3"); }
	else if (autonSelected == 4){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton4"); }
	else if (autonSelected == 5){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton5"); }
	else if (autonSelected == 6){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton6"); }
	else if (autonSelected == 7){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton7"); }
	else if (autonSelected == 8){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton8"); }
	else if (autonSelected == 9){ printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Auton9"); }
	else { printAtPoint(TEXT_LARGE_CENTER, 100, 100, "YOU'RE COOKED"); }
	*/

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
float tempvar = 0;
void opcontrol() {

	while (1){
		drive1.move_velocity(600 * tempvar);
		drive2.move_velocity(-600 * tempvar);
		if (controller.get_digital_new_press(DIGITAL_R1) == pressed){ // Toggles backclaw
            tempvar = abs(tempvar - 1.0);
        }
		delay(10);
	}

	//pros::lcd::initialize();
	//motorTesting ();
	

	//!screen::erase(); // Erases auton selector
	//!screen::set_pen(teamColor);
	//!screen::fill_rect(0,0,480,240);

	runDriveCont();
}
