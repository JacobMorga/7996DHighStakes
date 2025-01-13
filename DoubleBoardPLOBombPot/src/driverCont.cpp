#include "main.h"
using namespace pros;

float rightDrivePow, leftDrivePow;
float JRYValue, JRXValue, JLYValue, JLXValue;

bool buttonR2Prev = 0;

int intakeState = 0;
int exitcode = 0;
float sortDistance = 110.0;
float sortDelay1 = 150.0;
float sortDelay2 = 200.0;

float redLowLimit = 45.0;
float redHighLimit = 359.0;
float blueLowLimit = 45.1;
float blueHighLimit = 350.0;

/* school tuning
float redLowLimit = 48.999;
float redHighLimit = 359.0;
float blueLowLimit = 49.0;
float blueHighLimit = 200.0; //350
*/

// for new rgbc color sort, both are lower limits
const float redLimit = 180.0;
const float blueLimit = 85.0;

void runDriveCont (){

    while (1){

        JRYValue = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0; // Scales 127 to 100 then cubes and converts to mV
        JLYValue = powf(controller.get_analog(ANALOG_LEFT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JRXValue = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JLXValue = powf(controller.get_analog(ANALOG_LEFT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;

        rightDrivePow = JRYValue - JLXValue; // Calculates motor power
        leftDrivePow = JRYValue + JLXValue;

        rightDrive.move_voltage(rightDrivePow); // Sets motors to move
        leftDrive.move_voltage(leftDrivePow);

        /*
        if (abs(rightDrivePow) < 120.0 && fabs(leftDrivePow) < 120.0){ // Brakes drive when joytsick hits zero (1% power)
            rightDrive.brake();
            leftDrive.brake();
        }
        */

        /*
        if (intakeState == 2){intake.move_voltage(-13000.0);} // Spins backward
        else if (intakeState == 1){intake.move_voltage(13000.0);} // Spins forward
        else{intake.brake();} // Stops

        if (controller.get_digital_new_press(DIGITAL_R2) == pressed){ // Toggles between forward and backward with every press (active high)
            intakeState = abs(intakeState - 1);
        }
        if (controller.get_digital(DIGITAL_R1) == pressed){ // When R1 is pressing intakes spins backward
            intakeState = 2;
        }
        if (controller.get_digital(DIGITAL_R1) == unpressed && buttonR2Prev == pressed){ // When R1 is released intake off (active low)
            intakeState = 0;
        } 
        buttonR2Prev = controller.get_digital(DIGITAL_R1);
        */

        if (controller.get_digital_new_press(DIGITAL_L2)){backClaw.set_value(!backClaw.get_value());}
        if (controller.get_digital_new_press(DIGITAL_L1)){ploinker.set_value(!ploinker.get_value());}

        delay(10);
    }
}

std::string ringColor = "none";
std::string teamColor123 = "BLUE";
int var = 0;
void firstTryColorSorting (){
    pros::lcd::clear_line(3);

    while(1){
        opticalSensor.set_led_pwm(150);
        delay(10);
        pros::lcd::clear();
        intake.move_voltage(13000); // mV
        
        while (distanceSensor.get() < sortDistance){ // detected a new ring

            if(opticalSensor.get_hue() <= blueHighLimit && opticalSensor.get_hue() >= blueLowLimit){ // Ring is blue
                std::cout << "BLUE" << "\n";
                pros::lcd::set_text(0, "BLUE");
                ringColor = "BLUE";
            }
            else if(opticalSensor.get_hue() <= redLowLimit || opticalSensor.get_hue() >= redHighLimit){ // Ring is red
                std::cout << "RED" << "\n";
                pros::lcd::set_text(0, "RED");
                ringColor = "RED";
            }
            else{ // not red or blue
                std::cout << "none" << var << "\n";
                pros::lcd::set_text(0, "NONE");
                ringColor = "NONE";
            }
            std::cout << "HUE:" << opticalSensor.get_hue() << "\n";
            delay(10);
            var+=1;
            pros::lcd::clear();
            
            if (ringColor == "RED" && teamColor123 == "BLUE"){
                std::cout << "------ SORTING -------" << "\n";
                delay(sortDelay1);
                intake.move_voltage(-13000);
                delay(sortDelay2);
                intake.move_voltage(13000);

            }
        }
    }
}
void secondTryColorSorting (){
    
    while (distanceSensor.get() <= sortDistance){ // detected a ring
        if(opticalSensor.get_hue() <= blueHighLimit && opticalSensor.get_hue() >= blueLowLimit){ // Ring is blue
            ringColor = "BLUE";
        }
        else if(opticalSensor.get_hue() <= redLowLimit || opticalSensor.get_hue() >= redHighLimit){ // Ring is red
            ringColor = "RED";
        }
        else{ // not red or blue
            ringColor = "NONE";
        }
        if (ringColor == "RED" && teamColor123 == "BLUE"){
            delay(sortDelay1);
            intake.move_voltage(-13000);
            delay(sortDelay2);
            intake.move_voltage(13000);
        }
    }
    
}

/*
float colorSensed = 0.0;
float distanceSensed = 0.0;
void runIntake(){
    float intakeVoltage = 13000.0; // mV
    while(true){
        //pros::lcd::clear();
        if(controller.get_digital_new_press(DIGITAL_UP)){ // turns on and off light
            if(opticalSensor.get_led_pwm() >= 50){opticalSensor.set_led_pwm(0);}
            else{opticalSensor.set_led_pwm(100);}
        }
        if(controller.get_digital_new_press(DIGITAL_B)){
            if(teamColor == COLOR_RED){teamColor = COLOR_BLUE;}
            else{teamColor = COLOR_RED;}
        }
        if(distanceSensor.get() <= sortDistance && intakeState == 3){intakeState = 4;}

        if(controller.get_digital_new_press(DIGITAL_R1)){
            if(intakeState == 3){intakeState = 0;}
            else{intakeState = 3;}
        }
        if(controller.get_digital_new_press(DIGITAL_LEFT)){
            if(intakeState == 2){intakeState = 0;}
            else{intakeState = 2;}
        }
        if(controller.get_digital(DIGITAL_R2)){intakeState = 1;}
        if(controller.get_digital(DIGITAL_R2) == 0 && intakeState == 1){intakeState = 0;}

        if(intakeState == 0){intake.brake();}
        else if(intakeState == 1){intake.move_voltage(-intakeVoltage);}
        else if(intakeState == 2){intake.move_voltage(intakeVoltage);}
        else if(intakeState == 3){
            intake.move_voltage(intakeVoltage);
            opticalSensor.set_led_pwm(100);
        }
        else if(intakeState == 4){
            //std::cout << "RING ENTERED AT: " << distanceSensed << "\n";
            exitcode = 0;
            /*
            while(exitcode == 0 || (distanceSensor.get() <= sortDistance && ((teamBool == 0 && (opticalSensor.get_hue() <= blueLowLimit || opticalSensor.get_hue() >= blueHighLimit)) || (teamBool == 1 && opticalSensor.get_hue() >= redLowLimit && opticalSensor.get_hue() <= redHighLimit)) && controller.get_digital(DIGITAL_R1) == 0 && controller.get_digital(DIGITAL_R2) == 0 && controller.get_digital(DIGITAL_LEFT) == 0)){
                if(distanceSensor.get() > sortDistance){exitcode = 1;}
                if((teamBool == 0 && opticalSensor.get_hue() <= blueHighLimit && opticalSensor.get_hue() >= blueLowLimit) || (teamBool == 1 && (opticalSensor.get_hue() <= redLowLimit || opticalSensor.get_hue() >= redHighLimit))){exitcode = 2;}
                if(controller.get_digital(DIGITAL_R1)){exitcode = 3;}
                if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
                if(controller.get_digital(DIGITAL_LEFT)){exitcode = 5;}
                delay(10);
            }
            *
            while(exitcode == 0){
                colorSensed = opticalSensor.get_hue();
                distanceSensed = distanceSensor.get();
                if(distanceSensor.get() > sortDistance){exitcode = 1;}
                if((teamColor == COLOR_RED && colorSensed <= blueHighLimit && colorSensed >= blueLowLimit) || (teamColor == COLOR_BLUE && (colorSensed <= redLowLimit || colorSensed >= redHighLimit))){exitcode = 2;}
                if(controller.get_digital(DIGITAL_R1)){exitcode = 3;}
                if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
                if(controller.get_digital(DIGITAL_LEFT)){exitcode = 5;}
                //std::cout << "NOW CHECKING RING: " << colorSensed << "\n";
                //std::cout << "RING DISTANCE: " << distanceSensed << "\n";
                delay(10);
            }
            if(exitcode == 1){
                intakeState = 3;
                std::cout << "KEPT THIS ONE: " << colorSensed << "\n";
                //std::cout << "RING ESCAPED AT: " << distanceSensed << "\n" << "\n";
            }
            else if(exitcode == 2){
                delay(sortDelay1);
                intake.move_voltage(-intakeVoltage);
                delay(sortDelay2);
                intakeState = 3;
                std::cout << "CHUCKED THIS ONE: " << colorSensed << "\n";
                //std::cout << "RING IDENTIFIED AT: " << distanceSensed << "\n" << "\n";
            }
            else if(exitcode > 2){intakeState = exitcode - 3;}

            /*
            opticalSensor.set_led_pwm(100);
            pros::lcd::clear();
            pros::lcd::set_text(0, std::to_string(intakeState));
            pros::lcd::set_text(1, std::to_string(distanceSensor.get()));
            pros::lcd::set_text(2, std::to_string(opticalSensor.get_hue()));
            if(teamBool == 1){
                if(opticalSensor.get_hue() <= redLowLimit && opticalSensor.get_hue() >= redLowLimit1){
                    pros::lcd::set_text(3, "THIS LOW RED IS OUTTA HERE!");
                    delay(sortDelay1);
                    intake.move_voltage(-intakeVoltage);
                    delay(sortDelay2);
                }
                else if(opticalSensor.get_hue() <= redHighLimit2 && opticalSensor.get_hue() >= redHighLimit){
                    pros::lcd::set_text(3, "THIS HIGH RED IS OUTTA HERE!");
                    delay(sortDelay1);
                    intake.move_voltage(-intakeVoltage);
                    delay(sortDelay2);                    
                }
                intakeState = 3;
            }
            else{
                if(opticalSensor.get_hue() <= blueHighLimit && opticalSensor.get_hue() >= blueLowLimit){
                    pros::lcd::set_text(3, "THIS BLUE IS OUTTA HERE!");
                    delay(sortDelay1);
                    intake.move_voltage(-intakeVoltage);
                    delay(sortDelay2);
                }
                intakeState = 3;
            }
            *
            /*
            if((((opticalSensor.get_hue() >= redLowLimit1 && opticalSensor.get_hue() <= redLowLimit) || (opticalSensor.get_hue() >= redHighLimit && opticalSensor.get_hue() <= redHighLimit2)) && (teamBool == 1)) || ((opticalSensor.get_hue() >= blueLowLimit && opticalSensor.get_hue() <= blueHighLimit) && (teamBool == 1))){
                pros::lcd::set_text(3, "THIS RING IS OUTTA HERE!");
                delay(sortDelay1);
                intake.move_voltage(-intakeVoltage);
                delay(sortDelay2);
            }
            delay(2500);
            intakeState = 3;
            *
        }
        /*
        lcd::set_text(0, std::to_string(intakeState));
        lcd::set_text(1, std::to_string(distanceSensor.get()));
        lcd::set_text(2, std::to_string(opticalSensor.get_hue()));
        lcd::set_text(4, std::to_string(teamColor));
        lcd::set_text(6, std::to_string(exitcode));
        *
        delay(10);
    }
}
*/

float colorSensed = 0.0;
float distanceSensed = 0.0;
void runIntake(){
    pros::c::optical_raw_s_t raw_values;
    pros::c::optical_raw_s_t rawColors;
    float intakeVoltage = 11000.0; // mV
    while(true){

        //pros::lcd::clear();
        //! I JUST COMMENTED THIS 2 SECONDS BEFORE THE COMP
        /*
        if(controller.get_digital_new_press(DIGITAL_UP)){ // turns on and off light
            if(opticalSensor.get_led_pwm() >= 50){opticalSensor.set_led_pwm(0);}
            else{opticalSensor.set_led_pwm(100);}
        }
        if(controller.get_digital_new_press(DIGITAL_B)){
            if(teamColor == COLOR_RED){teamColor = COLOR_BLUE;}
            else{teamColor = COLOR_RED;}
        }
        */

        //? Intake State guide:
        //? 0: both stages stopped
        //? 1: reverse both stages
        //? 2: intake both stages blindly
        //? 3: intake both stages, old color sort (move to 4 when ring detected)
        //? 4: old color sort (move to 3 when exited)
        //? 5: intake both stages, then pause when ring detected (moves to 0)
        //? 6: intake top stage and reverse bottom stage
        //? 7: run top stage at 3V (brings ring to top of intake but doesn't score it)
        //? 8: intake both stages, slow down when detected (moves to 7)
        //? 9: intake both stages, new color sort (moves to 10 when ring detected)
        //? 10: new color sort (moves to 9 when exited)

        if(distanceSensor.get() <= sortDistance && intakeState == 3){intakeState = 4;} //old color sort
        if(distanceSensor.get() <= sortDistance && intakeState == 5){intakeState = 0;} //stop intake once ring detected
        if(distanceSensor.get() <= sortDistance && intakeState == 8){intakeState = 7;} //slow intake to 3V once ring detected
        if(distanceSensor.get() <= sortDistance && intakeState == 9){intakeState = 10;} //new color sort

        if(controller.get_digital_new_press(DIGITAL_R1)){
            if(intakeState == 9){intakeState = 0;}
            else{intakeState = 9;}
        }
        if(controller.get_digital_new_press(DIGITAL_LEFT)){
            if(intakeState == 2){intakeState = 0;}
            else{intakeState = 2;}
        }
        if(controller.get_digital_new_press(DIGITAL_DOWN)){
            if(intakeState == 3){intakeState = 0;}
            else{intakeState = 3;}
        }
        if(controller.get_digital(DIGITAL_R2)){intakeState = 1;}
        if(controller.get_digital(DIGITAL_R2) == 0 && intakeState == 1){intakeState = 0;}

        if(intakeState == 0){intake.brake();}
        else if(intakeState == 1){intake.move_voltage(-intakeVoltage); opticalSensor.set_led_pwm(0);}
        else if(intakeState == 2 || intakeState == 5 || intakeState == 8){intake.move_voltage(intakeVoltage); opticalSensor.set_led_pwm(0);}
        else if(intakeState == 6){intakeTop.move_voltage(intakeVoltage); intakeBottom.move_voltage(-intakeVoltage); opticalSensor.set_led_pwm(0);}
        else if(intakeState == 7){intakeTop.move_voltage(3000.0); opticalSensor.set_led_pwm(0);}
        else if(intakeState == 3 || intakeState == 9){
            intake.move_voltage(intakeVoltage);
            opticalSensor.set_led_pwm(100);
        }
        else if(intakeState == 4){
            //std::cout << "RING ENTERED AT: " << distanceSensed << "\n";
            exitcode = 0;
            while(exitcode == 0){
                raw_values = opticalSensor.get_raw();
                colorSensed = opticalSensor.get_hue();
                distanceSensed = distanceSensor.get();
                if(distanceSensor.get() > sortDistance){exitcode = 1;}
                if((teamColor == COLOR_RED && colorSensed <= blueHighLimit && colorSensed >= blueLowLimit) || (teamColor == COLOR_BLUE && (colorSensed <= redLowLimit || colorSensed >= redHighLimit))){exitcode = 2;}
                if(controller.get_digital(DIGITAL_R1)){exitcode = 3;}
                if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
                if(controller.get_digital(DIGITAL_LEFT)){exitcode = 5;}
                //std::cout << "NOW CHECKING RING: " << colorSensed << "\n";
                //std::cout << "RING DISTANCE: " << distanceSensed << "\n";
                //std::cout << "RED VAL: " << raw_values.red  << "\n";
                //std::cout << "BLUE VAL: " << raw_values.blue << "\n";
                //std::cout << "GREEN VAL: " << raw_values.green << "\n";
                delay(10);
            }
            if(exitcode == 1){
                intakeState = 3;
                std::cout << "KEPT THIS ONE: " << colorSensed << "\n";
                //std::cout << "RING ESCAPED AT: " << distanceSensed << "\n" << "\n";
            }
            else if(exitcode == 2){
                delay(sortDelay1);
                intake.move_voltage(-intakeVoltage);
                delay(sortDelay2);
                intakeState = 3;
                std::cout << "CHUCKED THIS ONE: " << colorSensed << "\n";
                //std::cout << "RING IDENTIFIED AT: " << distanceSensed << "\n" << "\n";
            }
            else if(exitcode > 2){intakeState = exitcode - 3;}
    
        }
        else if(intakeState == 10){
            exitcode = 0;
            while(exitcode == 0){
                rawColors = opticalSensor.get_raw();
                distanceSensed = distanceSensor.get();
                if(distanceSensor.get() > sortDistance){exitcode = 1;}
                else if((rawColors.red >= redLimit && teamColor == COLOR_BLUE) || (rawColors.blue >= blueLimit && teamColor == COLOR_RED)){exitcode = 2;}
                else if(controller.get_digital(DIGITAL_R1)){exitcode = 3;}
                else if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
                else if(controller.get_digital(DIGITAL_LEFT)){exitcode = 5;}
                delay(10);
            }
            if(exitcode == 1){intakeState = 9;}
            else if(exitcode == 2){
                delay(sortDelay1);
                intake.move_voltage(-intakeVoltage);
                delay(sortDelay2);
                intakeState = 9;
            }
            else if(exitcode > 2){intakeState = exitcode - 3;}
        }
        delay(10);
    }
}


//* COLOR CALIBRATION STEPS:
//* 1: Comment intake task in main.cpp >> initialize().
//* 2: Uncomment calibrate function in main.cpp >> opcontrol().
//* 3: Download and run with brain terminal printing to a new window.
//* 4: Feed ~20 rings of one color through the intake, verifying the output is printing to the terminal.
//*    Note: it might be helpful to feed them through while rotating the robot to face different directions.
//* 5: Copy output into a blank Google Sheet and auto-format the columns, which are distance and RGBC values.
//* 6: Repeat 3-5 with the other color.
//* 7: Make graphs of at least R and B outputs for both data sets.
//* 8: Identify minimum R and B values that indicate ring detection over ambient color or other ring color.
//* 9: Input these values into the variables redLimit and blueLimit above.
//* 10: Run test calibration function in main.cpp >> opcontrol(), ensuring brain terminal output functioning.
//* 11: Feed ~20 rings of each color through the intake, verifying correct identification.
//* 12: Repeat calibration if necessary.
//* 13: Comment both run and test calibration functions; uncomment intake task in main.cpp >> initialize().

void runColorCalibration(){
    pros::c::optical_raw_s_t rawVals;
	intake.move_voltage(11000.0);
	opticalSensor.set_led_pwm(100.0);
	float distanceMeasured = 0.0;
	while(1){
		rawVals = opticalSensor.get_raw();
		distanceMeasured = distanceSensor.get();
		std::cout << distanceMeasured << ", " << rawVals.red << ", " << rawVals.green << ", " << rawVals.blue << ", " << rawVals.clear << "\n";
		delay(10);
	}
}

void testColorCalibration(){
    pros::c::optical_raw_s_t rawVals;
	intake.move_voltage(11000.0);
	opticalSensor.set_led_pwm(100.0);
	float distanceMeasured = 0.0;
	float prevDistance = 0.0;
	int ringColor = 0;
	bool colorDecided = 0;
	while(1){
		rawVals = opticalSensor.get_raw();
		distanceMeasured = distanceSensor.get();
		if(distanceMeasured < 100){
			if(prevDistance >= 100){std::cout << "ring detected." << "\n";}
			if(colorDecided == 0 && rawVals.red >= redLimit){colorDecided = 1; ringColor = 1;}
			if(colorDecided == 0 && rawVals.blue >= blueLimit){colorDecided = 1; ringColor = 2;}
			std::cout << distanceMeasured << ", " << rawVals.red << ", " << rawVals.green << ", " << rawVals.blue << ", " << rawVals.clear << "\n";
		}
		else if(distanceMeasured >= 100 && prevDistance < 100){
			std::cout << "ring left." << "\n";
			if(colorDecided == 0){std::cout << "couldn't tell what color it was though." << "\n" << "\n";}
			else{
				if(ringColor == 1){std::cout << "it was a RED ring." << "\n" << "\n";}
				else if(ringColor == 2){std::cout << "it was a BLUE ring." << "\n" << "\n";}
			}
			colorDecided = 0;
			ringColor = 0;
		}
		prevDistance = distanceMeasured;
		delay(10);
	}
}