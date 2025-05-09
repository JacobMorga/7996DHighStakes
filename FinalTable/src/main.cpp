#include "main.h"
using namespace pros;

void initialize() {

	//Task LEDTask(DoTheLEDs, "LEDtask");
	//delay(100000000000);

	backClaw.set_value(0);

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
	yTracking.set_reversed(0);

	opticalSensor.set_integration_time(20);
	
	while(inertial1.is_calibrating() || inertial2.is_calibrating() || inertial3.is_calibrating()){delay(20);}
	delay(500);

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
	//Task LEDTask(DoTheLEDs, "LEDtask");
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
	WMForwardTarget = 2200.0;

	/*
	if(autonSelected == 1 || autonSelected == 2){teamColor = COLOR_BLUE;}
	else{teamColor = COLOR_RED;}
	*/

	screen::set_pen(COLOR_GREEN); // Colors brain screen with team color
	screen::fill_rect(0,0,480,240);
	screen::set_pen(COLOR_BLACK);
	screen::set_eraser(COLOR_GREEN);
	runThisCodeShort = 0;
	allowDebug = 0;
	
	if      (autonSelected == 1 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Blue Pos"); bluePos();} // Runs auton based on auton selector output
	else if (autonSelected == 2 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Blue Neg"); blueNeg();} // And prints what auton its running
	else if (autonSelected == 3 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Red Pos");  redPos();}
	else if (autonSelected == 4 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "Red Neg");  redNeg();}

	else if (autonSelected == 5 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BPAS NR");  bluePosNoRush();}
	else if (autonSelected == 6 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BNAS Cor"); blueNegAS();}
	else if (autonSelected == 7 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RPAS NR");  redPosNoRush();}
	else if (autonSelected == 8 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RNAS Cor"); redNegAS();}

	else if (autonSelected == 9 ){printAtPoint(TEXT_LARGE_CENTER, 180, 100, ""); }
	else if (autonSelected == 10){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BNAS Lad"); ladderTouch = 1; blueNegAS();}
	else if (autonSelected == 11){printAtPoint(TEXT_LARGE_CENTER, 180, 100, ""); }
	else if (autonSelected == 12){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RNAS Lad"); ladderTouch = 1; redNegAS();}

	else if (autonSelected == 13){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BPNS NR");  skipAS = 1; bluePosNoRush();}
	else if (autonSelected == 14){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "BNNS Lad"); skipAS = 1; ladderTouch = 1; blueNegAS();}
	else if (autonSelected == 15){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RPNS NR");  skipAS = 1; redPosNoRush();}
	else if (autonSelected == 16){printAtPoint(TEXT_LARGE_CENTER, 180, 100, "RNNS Lad"); skipAS = 1; ladderTouch = 1; redNegAS();}
	
	else {
		screen::set_pen(COLOR_RED); // Colors brain screen with team color
		screen::fill_rect(0,0,480,240);
		printAtPoint(TEXT_LARGE_CENTER, 100, 100, "YOU'RE COOKED");
	}
}

void opcontrol() {


	//drivetrain.set_brake_modes(MOTOR_BRAKE_COAST);
	//drivetrain.brake();

	/*
	float tmepvarbasd = 0;
	while(1){
		wallMech.move_voltage(tmepvarbasd);

		if (controller.get_digital_new_press(DIGITAL_R1)){ tmepvarbasd += 25;}
		if (controller.get_digital_new_press(DIGITAL_R2)){ tmepvarbasd -= 25;}

		lcd::set_text(0, std::to_string(tmepvarbasd));
		lcd::set_text(1, std::to_string(WMPotentiometer.get_value()));
		delay(10);
		lcd::clear();
	}
	*/

	//facePoint(48.0, 0.0, false, 6000.0);
	//toPoint(-48.0,48.0,0.0,true,12000.0);
	//toPoint(-96.0,0.0,0.0,true,12000.0);
	//toPoint(-48.0,-48.0,0.0,true,12000.0);
	//toPoint(0.0,0.0,0.0,false,12000.0);

	//purePursuit({coord(0.1,0.1), coord(-48.0, 48.0), coord(-96.0, 0.0)}, 24.0, 12000.0, 0); //get ring cube and go to top red ring

	if(driverSkills){
		transit(6);
		delay(500);
		transit(1);
		backClawBool = 0;
	}
	runDriveCont();

	/*
	int timevarcounter = 0;
	intake.move_voltage(13000.0);
	while(1){
		std::cout << "(" << timevarcounter << "," << intakeTop.get_current_draw() << ")" << "\n";
		timevarcounter += 10;
		delay(10);
	}
	*/

	//drivetrain.set_brake_modes(MOTOR_BRAKE_COAST);
	//drivetrain.brake();
	

	//calculateOffsets();
	

	/*
	intake.move_velocity(600.0);
	while(WMDistanceSensor.get() >= 100.0){
		delay(10);
	}
	intake.brake();
	delay(1000);
	intake.move_velocity(-600.0);
	delay(250);
	intake.brake();
	*/
	



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