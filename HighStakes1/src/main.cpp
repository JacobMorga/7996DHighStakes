#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
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

	pros::Task odomTask (odometry, "odomTask");

	delay(250);

	pros::Task taskDoing (doTask, "taskDoing");

	delay(250);

	pros::Task limitingSpeedandDrop (speedlimitanddrop, "limitingSpeedandDrop");

	delay(250);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit. 
 */
void disabled() { 
	
	drive1.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive2.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive3.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive4.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive5.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive6.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drivetrain.brake(); }

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
	//lastYear();
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
	//STARTTHETIMER = true;
	//redgoal();
	rednone();
	//bluegoal();
	//bluenone();

	//toPoint(0.0, 24.0);

	/*
    inputvovfp = 1;
    rotkp = 500.0; //150.0; //160.0; //150
    rotki = 81.6922; //!suggested next tune: 77.25221312
    rotkd = 3350.0; //375.0; //1600
    lcd::set_text(7, "running auton");
	facepoint(0.0, -24.0);
	delay(1000);
	facepoint(-24.0, 0.0);
	delay(1000);
	facepoint(0.0, 24.0);
    lcd::set_text(7, "ended auton");
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
void opcontrol() {

	//delay(100000);
	
	/*
    pros::screen_touch_status_s_t status;
	delay(200);
	screen::erase();
	delay(200);
    while(1){
       status = pros::screen_touch_status_s();

       // Will print various information about the last touch
       pros::screen::print(pros::E_TEXT_MEDIUM, 1, "Touch Status (Type): %d", status.touch_status);
       pros::screen::print(pros::E_TEXT_MEDIUM, 2, "Last X: %d", status.x);
       pros::screen::print(pros::E_TEXT_MEDIUM, 3, "Last Y: %d", status.y);
       pros::screen::print(pros::E_TEXT_MEDIUM, 4, "Press Count: %d", status.press_count);
       pros::screen::print(pros::E_TEXT_MEDIUM, 5, "Release Count: %d", status.release_count);
       pros::delay(20);
	}

	DisplayBox box1 (0,0,120,239,0,2);
	box1.setButtons({COLOR_BLUE, COLOR_RED}, false);

	DisplayBox box2 (120,0,360,239,3,3);
	box1.setButtons({COLOR_GREEN}, true);

	DisplayBox box3 (360,0,479,239,1,1);

	vector<DisplayBox> testingRects {box1,box2,box3};
	autonSelectorNEW(testingRects);
	*/
	

	
	/*
	toPoint(-24.0, 72.0);
	toPoint(-96.0, 48.0);
	toPoint(-72.0, -24.0);
	toPoint(0.0, 0.0);
	*/
	//rotlinrot(-24.0, 72.0, 0.0);
	//toPoint(0.0, 48.0);
	//rotarc(-24.0, 48.0, -pi / 4.0);
	//facepoint(24.0, 0.0);
	//toPoint(24.0, -72.0);

	ploinker.set_value(0);
	while (true) {
		runDriveCont();
		delay(20);
	}
	
}
