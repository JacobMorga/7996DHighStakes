#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
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
void competition_initialize() {}

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
void autonomous() {}

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
	/*

	

	DisplayBox box1 (0,0,120,239,0,2);
	box1.setButtons({COLOR_BLUE, COLOR_RED}, false);

	DisplayBox box2 (120,0,360,239,3,3);
	box1.setButtons({COLOR_GREEN}, true);

	DisplayBox box3 (360,0,479,239,0,0);

	vector<DisplayBox> testingRects {box1,box2,box3};
	autonSelectorNEW(testingRects);
	*/
	inertial1.reset();
	yTracking.reset();

	delay(3000);

	pros::Task odomTask (odometry, "odomTask");

	while (true) {

		rightDrive.move_velocity(controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)/127*600);
		leftDrive.move_velocity(controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)/127*600);

		intake.move_velocity(600 * (controller.get_digital(E_CONTROLLER_DIGITAL_R1) - controller.get_digital(E_CONTROLLER_DIGITAL_R2)));
		
		pros::delay(20);
	}
	
}
