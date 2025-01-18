#include "main.h"
using namespace pros;

float JRYValue, JRXValue, JLYValue, JLXValue;

int intakeState = 0;
int exitcode = 0;
float sortDistance = 110.0;
float sortDelay1 = 150.0;
float sortDelay2 = 200.0;

const float redLimit = 180.0;
const float blueLimit = 85.0;

void runDriveCont (){
    while (1){
        JRYValue = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0; // Scales 127 to 100 then cubes and converts to mV
        JLYValue = powf(controller.get_analog(ANALOG_LEFT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JRXValue = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JLXValue = powf(controller.get_analog(ANALOG_LEFT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;

        rightDrive.move_voltage(JRYValue - JLXValue); // Sets motors to move
        leftDrive.move_voltage(JRYValue + JLXValue);

        if (controller.get_digital_new_press(DIGITAL_L2)){backClaw.set_value(!backClaw.get_value());}
        if (controller.get_digital_new_press(DIGITAL_L1)){clearer.set_value(!clearer.get_value());}

        delay(10);
    }
}

float distanceSensed = 0.0;
void runIntake(){
    pros::c::optical_raw_s_t rawColors = opticalSensor.get_raw();
    float intakeVoltage = 11000.0; // mV
    while(true){

        //& New Intake State Guide:
        //& 0: both stages stopped
        //& 1: reverse both stages
        //& 2: intake both stages blindly
        //& 3: intake with color sort
        //& 4: perform color sort
        //& 5: intake until ring detected

        if(distanceSensor.get() <= sortDistance){
            if (intakeState == 3){intakeState = 4;} //color sort
            else if (intakeState == 5){intakeState = 0;} //pause when ring seen
        }

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

        if(intakeState == 0){intake.brake(); opticalSensor.set_led_pwm(0);}
        else if(intakeState == 1){intake.move_voltage(-intakeVoltage); opticalSensor.set_led_pwm(0);}
        else if(intakeState == 2 || intakeState == 5){intake.move_voltage(intakeVoltage); opticalSensor.set_led_pwm(0);}
        else if(intakeState == 3){
            intake.move_voltage(intakeVoltage);
            opticalSensor.set_led_pwm(100);
        }
        else if(intakeState == 4){
            exitcode = 0;
            while(exitcode == 0){
                rawColors = opticalSensor.get_raw();
                std::cout << rawColors.red << ", " << rawColors.blue;
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
                std::cout << "sorted this one out";
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
    pros::c::optical_rgb_s_t rawVals;
	intake.move_voltage(11000.0);
	opticalSensor.set_led_pwm(100.0);
	float distanceMeasured = 0.0;
	while(1){
		rawVals = opticalSensor.get_rgb();
		distanceMeasured = distanceSensor.get();
		std::cout << distanceMeasured << ", " << rawVals.red / opticalSensor.get_brightness() << ", " << rawVals.green / opticalSensor.get_brightness() << ", " << rawVals.blue / opticalSensor.get_brightness() << "\n";
        lcd::set_text(6, std::to_string(rawVals.red));
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
