#include "main.h"
using namespace pros;

float JRYValue, JRXValue, JLYValue, JLXValue; //joystick right y, right x, left y, left x values

int intakeState = 0; //current intake state (0-5)
float intakeVoltage = 11000.0; // mV
int exitcode = 0; //indicator for why color sorting state (4) exited
float sortDistance = 110.0; //mm
float sortDelay1 = 150.0; //ms
float sortDelay2 = 200.0; //ms

const float redLimit = 180.0; //lower limits for rgbc sort
const float blueLimit = 85.0;

int wallMechState = 0; //current wall mech state (0-3)
float idleHighLimit = 20.0; //upper limit for potentiometer value in idle state
float loadingALowLimit = 50.0; //lower limit for potentiometer value in loading A state
float loadingAHighLimit = 51.0; //upper limit for potentiometer value in loading A state
float loadingBLowLimit = 60.0; //lower limit for potentiometer value in loading B state
float loadingBHighLimit = 61.0; //upper limit for potentiometer value in loading B state
float scoringLowLimit = 250.0; //lower limit for potentiometer value in scoring state
float wallMechVoltage = 12000.0; //mV

float wallMechTarget = 0.0; // Target position for wall mech arm (Degrees)
float WMError = 0.0; // Wall mech error
float WMKp = 100.0; // tuned value for wall mech P-loop
float WMKd = 700.0;
float WMPosition = 0.0;
float WMPreviousPos = 0.0;
float WMDerivative = 0.0;
float WMPower = 0.0;

void runDriveCont (){
    while (1){
        JRYValue = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0; // Scales 127 to 100 then cubes and converts to mV
        JLYValue = powf(controller.get_analog(ANALOG_LEFT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JRXValue = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JLXValue = powf(controller.get_analog(ANALOG_LEFT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;

        rightDrive.move_voltage(JRYValue - JLXValue); // Sets motors to move
        leftDrive.move_voltage(JRYValue + JLXValue);

        if (controller.get_digital_new_press(DIGITAL_L2)){backClaw.set_value(!backClaw.get_value());} //toggles back claw
        if (controller.get_digital_new_press(DIGITAL_LEFT)){leftClearer.set_value(!leftClearer.get_value());} //toggles left clearer
        if (controller.get_digital_new_press(DIGITAL_UP)){rightClearer.set_value(!rightClearer.get_value());} //toggles right clearer

        delay(10);
    }
}

float distanceSensed = 0.0;
void runIntakeAndWallMech(){
    pros::c::optical_raw_s_t rawColors = opticalSensor.get_raw();
    while(true){

        //& Intake State Guide:
        //& 0: both stages stopped
        //& 1: reverse both stages
        //& 2: intake both stages blindly
        //& 3: intake with color sort
        //& 4: perform color sort
        //& 5: intake until ring detected

        //* intake state transition control
        if(distanceSensor.get() <= sortDistance){ //when ring seen
            if (intakeState == 3){intakeState = 4;} //color sort
            else if (intakeState == 5){intakeState = 0;} //pause
        }
        if(controller.get_digital_new_press(DIGITAL_R1)){ //toggle color sort and off
            if(intakeState == 3){intakeState = 0;}
            else{intakeState = 3;}
        }
        if(controller.get_digital_new_press(DIGITAL_LEFT)){ //toggle blind intake and off
            if(intakeState == 2){intakeState = 0;}
            else{intakeState = 2;}
        }
        if(controller.get_digital(DIGITAL_R2)){intakeState = 1;} //button hold to outtake
        if(controller.get_digital(DIGITAL_R2) == 0 && intakeState == 1){intakeState = 0;}

        //* intake state execution control
        if(intakeState == 3 || intakeState == 4){opticalSensor.set_led_pwm(100);} //optical led control
        else{opticalSensor.set_led_pwm(0);}

        if(intakeState == 0){intake.brake();} //0: off
        else if(intakeState == 1){intake.move_voltage(-intakeVoltage);} //1: reverse
        else if(intakeState == 2 || intakeState == 3 || intakeState == 5){intake.move_voltage(intakeVoltage);} //2: blind intake; 3: color sort; 5: wait until ring
        else if(intakeState == 4){ //4: color sorting
            exitcode = 0;
            while(exitcode == 0){
                rawColors = opticalSensor.get_raw();
                std::cout << rawColors.red << ", " << rawColors.blue;
                distanceSensed = distanceSensor.get();
                if(distanceSensed > sortDistance){exitcode = 1;} //ring passed color sorting
                else if((rawColors.red >= redLimit && teamColor == COLOR_BLUE) || (rawColors.blue >= blueLimit && teamColor == COLOR_RED)){exitcode = 2;} //ring flagged color sorting
                else if(controller.get_digital(DIGITAL_R1)){exitcode = 3;} //exit to corresponding state
                else if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
                else if(controller.get_digital(DIGITAL_LEFT)){exitcode = 5;}
                delay(10);
            }
            if(exitcode == 1){intakeState = 3;} //ring passed color sorting
            else if(exitcode == 2){ //sort flagged ring
                delay(sortDelay1);
                intake.move_voltage(-intakeVoltage);
                delay(sortDelay2);
                intakeState = 3;
            }
            else if(exitcode > 2){intakeState = exitcode - 3;} //button exit to corresponding state
        }


        //& Wall Mech State Guide:
        //& 0: idle
        //& 1: loading A
        //& 2: loading B
        //& 3: scoring

        //* wall mech state transition control
        if(controller.get_digital_new_press(DIGITAL_L1)){ //forward one state
            if(wallMechState < 3){wallMechState += 1;}
            else{wallMechState = 0;}
        }

        /*
        //* wall mech state execution control
        if(wallMechState == 0){ //move until in idle zone
            if(wallMechPotentiometer.get_angle() > idleHighLimit){wallMech.move_voltage(-wallMechVoltage);}
            else{wallMech.brake();}
        }
        else if(wallMechState == 1){ //move until in loading A zone
            if(wallMechPotentiometer.get_angle() < loadingALowLimit){wallMech.move_voltage(wallMechVoltage);}
            else if(wallMechPotentiometer.get_angle() > loadingAHighLimit){wallMech.move_voltage(-wallMechVoltage);}
            else{wallMech.brake();}
        }
        else if(wallMechState == 2){ //move until in loading B zone
            if(wallMechPotentiometer.get_angle() < loadingBLowLimit){wallMech.move_voltage(wallMechVoltage);}
            else if(wallMechPotentiometer.get_angle() > loadingBHighLimit){wallMech.move_voltage(-wallMechVoltage);}
            else{wallMech.brake();}
        }
        else if(wallMechState == 3){ //move until in scoring zone
            if(wallMechPotentiometer.get_angle() < scoringLowLimit){wallMech.move_voltage(wallMechVoltage);}
            else{wallMech.brake();}
        }
        */

        //$ Wall Mech Code - runs one step of loop every driver cont

        if (wallMechState == 0) { wallMechTarget = 2.0; }
        else if (wallMechState == 1) { wallMechTarget = 100.0; }
        else if (wallMechState == 2) { wallMechTarget = 150.0; }
        else if (wallMechState == 3) { wallMechTarget = 410.0; }

        WMPosition = wallMech.get_position();
        WMError = wallMechTarget - WMPosition;
        WMDerivative = WMPreviousPos - WMPosition;
        WMPower = WMKp * WMError + WMKd * WMDerivative;
        WMPreviousPos = WMPosition;
        lcd::set_text(4, std::to_string(WMError * WMKp));
        lcd::set_text(5, std::to_string(WMError));
        lcd::set_text(6, std::to_string(wallMechState));
        lcd::set_text(7, std::to_string(wallMech.get_position()));

        //if (fabs(WMError) > 3.0){ wallMech.move_voltage(WMPower); } // If error is over 3 degrees then move
        //else { wallMech.brake(); } // Should be hold type of brake
        wallMech.move_voltage(WMPower);
        if(controller.get_digital(DIGITAL_DOWN)){wallMech.move_voltage(0.0);}

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
