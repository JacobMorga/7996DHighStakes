#include "main.h"
using namespace pros;

float JRYValue, JRXValue, JLYValue, JLXValue; //joystick right y, right x, left y, left x values

int intakeState = 0; //current intake state (0-5)
float intakeVoltage = 11000.0; // mV
int exitcode = 0; //indicator for why color sorting state (4) exited
float sortDistance = 20.0; //110.0; //mm
float sortDelay1 = 150.0; //ms
float sortDelay2 = 200.0; //ms

const float redLimit = 180.0; //lower limits for rgbc sort
const float blueLimit = 85.0;

float ambient = 0.0;
float redFactor = 0.0;
float blueFactor = 0.0;

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
float WMKp = 150.0; //100.0; // tuned value for wall mech P-loop
float WMKd = 0.0;
float WMKdForward = 400.0; //700
float WMKdBackward = 1000.0;
float WMPosition = 0.0;
float WMPreviousPos = 0.0;
float WMDerivative = 0.0;
float WMPower = 0.0;
float WMKi = 0.0;
float WMKiForward = 1.0;
float WMKiBackward = 10.0;
float WMIntegral = 0.0;
float WMErrorMax = 200.0;
float WMErrorMin = 1.0;
float WMIntegralMax = 4000.0;
float WMTargetAdjustment = 0.0;
float WMIdleTarget = 2.0;
float WMLoadingTarget = 125.0;
float WMLoadingBTarget = 180.0; //unused and untested
float WMScoringTarget = 420.0;
float WMAdjustmentIncrement = 10.0;
float WMRingDetectionDist = 90.0;

bool colorSorting = 1;

void runDriveCont (){

    //$ Controller mapping:
    //$ Button: Current assignment              -> Planned future assignment
    //$ Left joystick X axis: Turning
    //$ Left joystick Y axis:
    //$ Right joystick X axis:
    //$ Right joystick Y axis: Driving
    //$ L1: Left corner clearer toggle          -> Wall mech state cycle
    //$ L2: Back claw toggle
    //$ R1: Color sort intake toggle            -> "Run intake" following color sort on/off toggle
    //$ R2: Reverse intake, hold button         //this might switch out because some stuff is more likely to be used than outtaking
    //$ Up: Wall mech manual adjust up
    //$ Down: Wall mech manual adjust down
    //$ Left: Wall mech state cycle             -> Left corner clearer toggle
    //$ Right: Blind intake toggle              -> Color sorting on/off toggle
    //$ X (up): Intake piston toggle            -> Intake until ring detected
    //$ Y (left): Right corner clearer toggle   -> Intake piston toggle
    //$ A (right): Intake until ring detected   -> Right corner clearer toggle
    //$ B (down): Wall mech go limp, hold button
    //$ Potential additional adjustments/additions:
    //$ instead of one button for the wall mech use three and assign a state to each one so you never
    //$     have to double click to go from scoring to loading (picking up another ring), from loading
    //$     to idle (changed your mind), or from idle to scoring (climb)
    //$ some kind of macro for holding a second ring in the intake and scoring it on the wall stake
    //$     after the wall mech ring is scored
    //$ direct climb button instead of double wall mech state cycle (basically combined with wall mech 3 button)
    //$ rework up/down arrows to enter a "manual mode" on the wall mech when either is pressed and where
    //$     it's either braking (neither pressed) or moving at a constant speed slowly, then if regular
    //$     wall mech cycle/state buttons are used manual mode is exited

    while (1){
        JRYValue = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0; // Scales 127 to 100 then cubes and converts to mV
        JLYValue = powf(controller.get_analog(ANALOG_LEFT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JRXValue = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JLXValue = powf(controller.get_analog(ANALOG_LEFT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;

        rightDrive.move_voltage(JRYValue - JLXValue); // Sets motors to move
        leftDrive.move_voltage(JRYValue + JLXValue);

        if(controller.get_digital_new_press(DIGITAL_L2)){backClaw.set_value(!backClaw.get_value());} //toggles back claw
        if(controller.get_digital_new_press(DIGITAL_X)){intakePiston.set_value(!intakePiston.get_value());} //toggles intake piston
        if(controller.get_digital_new_press(DIGITAL_L1)){leftClearer.set_value(!leftClearer.get_value());} //toggles left clearer
        if(controller.get_digital_new_press(DIGITAL_Y)){rightClearer.set_value(!rightClearer.get_value());} //toggles right clearer
        if(controller.get_digital_new_press(DIGITAL_RIGHT)){colorSorting = !colorSorting;} //toggles color sorting on/off

        /*
        lcd::set_text(0, std::to_string(xPos));
        lcd::set_text(1, std::to_string(yPos));
        lcd::set_text(2, std::to_string(getAngle()));
        */
        

}

float distanceSensed = 0.0;
bool ringYet = 0;
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
        //& 6: hooks intaking, flex wheels outtaking

        //* intake state transition control
        if(distanceSensor.get() <= sortDistance){ //when ring seen
            if (intakeState == 3){intakeState = 4;} //color sort
            else if (intakeState == 5){intakeState = 0;} //pause
        }
        if(controller.get_digital_new_press(DIGITAL_R1)){ //toggle color sort and off
            if(intakeState == 2 || intakeState == 5){intakeState = 0;} //! change 2 to 3 later for sorting
            else{intakeState = 2;}
            ringYet = 0;
        }
        /*
        if(controller.get_digital_new_press(DIGITAL_RIGHT)){ //toggle blind intake and off
            if(intakeState == 2){intakeState = 0;}
            else{intakeState = 2;}
        }
        */
        if(controller.get_digital(DIGITAL_R2)){intakeState = 1;} //button hold to outtake
        if(controller.get_digital(DIGITAL_R2) == 0 && intakeState == 1){intakeState = 0;}
        if(controller.get_digital(DIGITAL_A)){intakeState = 5;} //pull ring onto intake then wait

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
                ambient = opticalSensor.get_brightness();
                redFactor = rawColors.red / ambient;
                blueFactor = rawColors.blue / ambient;
                distanceSensed = distanceSensor.get();
                if(distanceSensed > sortDistance){exitcode = 1;} //ring passed color sorting
                else if((redFactor >= redLimit && teamColor == COLOR_BLUE) || (blueFactor >= blueLimit && teamColor == COLOR_RED)){exitcode = 2;} //ring flagged color sorting
                else if(controller.get_digital(DIGITAL_R1)){exitcode = 3;} //exit to corresponding state
                else if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
                else if(controller.get_digital(DIGITAL_A)){exitcode = 5;}
                std::cout << ambient << ", " << rawColors.red << ", " << rawColors.blue << ", " << redFactor << ", " << blueFactor;
                delay(10);
            }
            if(exitcode == 1){intakeState = 3;} //ring passed color sorting
            else if(exitcode == 2){ //sort flagged ring
                if(wallMechState == 1){
                    //wall mech dodge
                    delay(1); //just so this if statement isnt empty
                }
                delay(sortDelay1);
                intake.move_voltage(-intakeVoltage);
                delay(sortDelay2);
                intakeState = 3;
            }
            else if(exitcode > 2){intakeState = exitcode - 3;} //button exit to corresponding state
        }
        else if(intakeState == 6){intakeTop.move_voltage(intakeVoltage); intakeBottom.move_voltage(-intakeVoltage);}


        //& Wall Mech State Guide:
        //& 0: idle
        //& 1: loading
        //& 2: scoring (was loading B)
        //& 3: unused (was scoring)
        //& 4: shifted ±10n from state 1 or 2

        //* wall mech state transition control
        if(controller.get_digital_new_press(DIGITAL_LEFT)){ //forward one state
            //if(wallMechState < 2){wallMechState += 1;} //! should be 3 when we have the second ring thing
            //else{wallMechState = 0;}
            if(wallMechState == 0){wallMechState = 1; WMIntegral = 0.0;}
            else if(wallMechState == 1){wallMechState = 2; WMIntegral = 0.0;}
            else if(wallMechState == 2){wallMechState = 0; WMIntegral = 0.0;}
            else if(wallMechState == 4){wallMechState = 2; WMIntegral = 0.0;}
            WMIntegral = 0.0;

            if(wallMechState == 1){intakeState = 2;}
            else if(wallMechState == 2){ringYet = 0;}
        }

        /*
        if(controller.get_digital_new_press(DIGITAL_UP) && wallMechState != 0){wallMechState = 4; WMTargetAdjustment += 1;}
        if(controller.get_digital_new_press(DIGITAL_DOWN) && wallMechState != 0){wallMechState = 4; WMTargetAdjustment -= 1;}
        */

        if(controller.get_digital_new_press(DIGITAL_UP)){
            if(wallMechState == 1){WMLoadingTarget += WMAdjustmentIncrement;}
            else if(wallMechState == 2){WMScoringTarget += WMAdjustmentIncrement;}
        }
        if(controller.get_digital_new_press(DIGITAL_DOWN)){
            if(wallMechState == 1){WMLoadingTarget -= WMAdjustmentIncrement;}
            else if(wallMechState == 2){WMScoringTarget -= WMAdjustmentIncrement;}
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

        if (wallMechState == 0) { wallMechTarget = WMIdleTarget;}
        else if (wallMechState == 1){
            wallMechTarget = WMLoadingTarget;
            if(WMDistanceSensor.get() < WMRingDetectionDist && ringYet == 0){
                ringYet = 1;
                delay(50);
                intake.move_voltage(-intakeVoltage);
                delay(100);
                intake.move_voltage(0.0);
                intakeState = 0;
            }
        }
        //else if (wallMechState == 2) { wallMechTarget = WMLoadingBTarget; }
        else if(wallMechState == 2){wallMechTarget = WMScoringTarget;} //max 440? do we need wall mech calibration 💀 //460 //max 480

        WMPosition = wallMech.get_position();
        WMError = wallMechTarget - WMPosition;
        WMDerivative = WMPreviousPos - WMPosition;
        WMIntegral += WMError;
        if(wallMechTarget < 250.0){WMKi = WMKiForward; WMKd = WMKdForward;}
        else{WMKi = WMKiBackward; WMKd = WMKdBackward;}
        if(wallMechState == 0){WMKi = 0.0;}
        //if (fabs(WMError) >= WMErrorMax){WMIntegral = 0.0;} //not sure if this is ever actually helpful
        if (fabs(WMError) <= WMErrorMin){WMIntegral = 0.0;} //this is to stop the 瑟瑟发抖 //did we ever test this cause still big time 瑟瑟发抖
        if (fabs(WMIntegral * WMKi) >= WMIntegralMax){WMIntegral = getDir(WMIntegral) * WMIntegralMax / WMKi;}
        WMPower = WMKp * WMError + WMKd * WMDerivative + WMKi * WMIntegral;
        WMPreviousPos = WMPosition;

        /*
        //lcd::set_text(4, std::to_string(backClaw.get_value()));
        lcd::set_text(4, std::to_string(WMPower / 1000.0));
        lcd::set_text(5, std::to_string(WMError));
        lcd::set_text(6, std::to_string(WMIntegral * WMKi));
        lcd::set_text(7, std::to_string(wallMech.get_position()));
        */

        //if (fabs(WMError) > 3.0){ wallMech.move_voltage(WMPower); } // If error is over 3 degrees then move
        //else { wallMech.brake(); } // Should be hold type of brake
        wallMech.move_voltage(WMPower);
        //if(controller.get_digital(DIGITAL_B)){wallMech.move_voltage(0.0);}

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

pros::c::optical_raw_s_t rawColors;
pros::c::optical_rgb_s_t rgbColors;
float measuredHue;
float ambientBrightness;
float distanceMeasured;

void runColorCalibration(){
    /*
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
    */

    opticalSensor.set_led_pwm(100.0);
    intake.move_voltage(12000.0);
    while(1){
        rawColors = opticalSensor.get_raw();
        rgbColors = opticalSensor.get_rgb();
        measuredHue = opticalSensor.get_hue();
        ambientBrightness = opticalSensor.get_brightness();
        distanceMeasured = distanceSensor.get();
        std::cout << distanceMeasured << ", " << ambientBrightness << ", " << rawColors.red << ", " << rawColors.blue << ", " << rgbColors.red << ", " << rgbColors.blue << ", " << measuredHue << ", " << rawColors.red / ambientBrightness << ", " << rawColors.blue / ambientBrightness << ", " << rawColors.red * ambientBrightness << ", " << rawColors.blue * ambientBrightness << ", " << rgbColors.red / ambientBrightness << ", " << rgbColors.blue / ambientBrightness << ", " << rgbColors.red * ambientBrightness << ", " << rgbColors.blue * ambientBrightness;
        //there's a ton of variables in this printing line so we can just make a million graphs in the excel and see what works best, i have no idea if these multiplying or dividing things will work but we'll see
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

/*
int comboState = 0;
float comboStateDecimal = 0.0;

void runComboSystem(){
    //^ Combo state guide:
    //^ 0: intake off, wall mech idle
    //^ 1: intaking, wall mech idle (color sort controlled by separate toggle, all still state 1)
    //^ 2: intake reversing, wall mech idle
    //^ 3: intaking until ring detected, wall mech idle (color sort by toggle)
    //^ 4: intaking, wall mech loading (color sort by toggle)
    //^ 5: intake stopped, wall mech loaded but down
    //^ 6: intake stopped, wall mech in scoring position
    //^ 7: intaking, wall mech scoring (color sort by toggle)
    //^ 8: intake reversing, wall mech scoring
    //^ 9: intaking until ring detected, wall mech scoring (color sort by toggle)

    //^ 0.0: intake off, wall mech idle
    //^ 0.1: intake stopped, wall mech loaded but down
    //^ 0.2: intake stopped, wall mech in scoring position
    //^ 1.0: intaking, wall mech idle (color sort controlled by separate toggle, all still state 1)
    //^ 1.1: intaking, wall mech loading (color sort by toggle)
    //^ 1.2: intaking, wall mech scoring (color sort by toggle)
    //^ 2.0: intake reversing, wall mech idle
    //^ 2.2: intake reversing, wall mech scoring
    //^ 3.0: intaking until ring detected, wall mech idle (color sort by toggle)
    //^ 3.2: intaking until ring detected, wall mech scoring (color sort by toggle)

    while(1){
        //* state transitions
        if(controller.get_digital_new_press(DIGITAL_R1)){ //pressed to start intaking
            if(comboState == 0){comboState = 1;}
            else if(comboState == 1){comboState = 0;}
            else if(comboState == 2){comboState = 1;} //shouldn't really happen
            else if(comboState == 3){comboState = 0;}
            else if(comboState == 4){comboState = 0;} //this one should never occur because R1 has to be held to be in state 4
            else if(comboState == 5){comboState = 7;} //if you just put a ring in the wall mech then hit start intake it will move the wall mech out of the way first (hopefully)
            else if(comboState == 6){comboState = 7;}
            else if(comboState == 7){comboState = 6;}
            else if(comboState == 8){comboState = 7;} //shouldn't really happen
            else if(comboState == 9){comboState = 6;}
        }
        else if(controller.get_digital(DIGITAL_R1)){ //holding intake
            if(comboState == 0){comboState = 4;}
            else if(comboState == 1){comboState = 4;}
            else if(comboState == 2){comboState = 4;}
            else if(comboState == 3){comboState = 4;}
            else if(comboState == 4){comboState = 4;}
            else if(comboState == 5){comboState = 5;}
            else if(comboState == 6){comboState = 4;}
            else if(comboState == 7){comboState = 4;}
            else if(comboState == 8){comboState = 4;}
            else if(comboState == 9){comboState = 4;}
        }
        else if(controller.get_digital(DIGITAL_R1) == 0){ //intake button is simply not being pressed
            if(comboState == 0){comboState = 0;}
            else if(comboState == 1){comboState = 1;}
            else if(comboState == 2){comboState = 2;}
            else if(comboState == 3){comboState = 3;}
            else if(comboState == 4){comboState = 0;}
            else if(comboState == 5){comboState = 5;}
            else if(comboState == 6){comboState = 6;}
            else if(comboState == 7){comboState = 7;}
            else if(comboState == 8){comboState = 8;}
            else if(comboState == 9){comboState = 9;}
        }


    }
}
*/

void runIntake(){
    while(1){

        //& Intake State Guide:
        //& 0: both stages stopped
        //& 1: reverse both stages
        //& 2: intake both stages blindly
        //& 3: intake with color sort
        //& 4: perform color sort
        //& 5: intake until ring detected
        //& 6: hooks intaking, flex wheels outtaking
        
        //* intake state transition control
        if(distanceSensor.get() <= sortDistance){ //when ring seen
            if(intakeState == 3){intakeState = 4;} //color sort
            else if(intakeState == 5){intakeState = 0;} //pause
        }
        if(controller.get_digital_new_press(DIGITAL_R1)){ //toggle intake knowing color sort bool
            if(intakeState == 2 || intakeState == 3 || intakeState == 5){intakeState = 0;}
            else{if(colorSorting){intakeState = 3;} else{intakeState = 2;}}
            ringYet = 0;
        }

        if(controller.get_digital(DIGITAL_R2)){intakeState = 1;} //button hold to outtake
        if(controller.get_digital(DIGITAL_R2) == 0 && intakeState == 1){intakeState = 0;}
        if(controller.get_digital(DIGITAL_A)){intakeState = 5;} //pull ring onto intake then wait        
        delay(10);
    }
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
                ambient = opticalSensor.get_brightness();
                redFactor = rawColors.red / ambient;
                blueFactor = rawColors.blue / ambient;
                distanceSensed = distanceSensor.get();
                if(distanceSensed > sortDistance){exitcode = 1;} //ring passed color sorting
                else if((redFactor >= redLimit && teamColor == COLOR_BLUE) || (blueFactor >= blueLimit && teamColor == COLOR_RED)){exitcode = 2;} //ring flagged color sorting
                else if(controller.get_digital(DIGITAL_R1)){exitcode = 3;} //exit to corresponding state
                else if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
                else if(controller.get_digital(DIGITAL_A)){exitcode = 5;}
                std::cout << ambient << ", " << rawColors.red << ", " << rawColors.blue << ", " << redFactor << ", " << blueFactor;
                delay(10);
            }
            if(exitcode == 1){intakeState = 3;} //ring passed color sorting
            else if(exitcode == 2){ //sort flagged ring
                if(wallMechState == 1){
                    //wall mech dodge
                    delay(1); //just so this if statement isnt empty
                }
                delay(sortDelay1);
                intake.move_voltage(-intakeVoltage);
                delay(sortDelay2);
                intakeState = 3;
            }
            else if(exitcode > 2){intakeState = exitcode - 3;} //button exit to corresponding state
        }
        else if(intakeState == 6){intakeTop.move_voltage(intakeVoltage); intakeBottom.move_voltage(-intakeVoltage);}
        delay(10);
    }
}

void runWallMech(){

        //& Wall Mech State Guide:
        //& 0: idle
        //& 1: loading
        //& 2: scoring (was loading B)
        //& 3: unused (was scoring)
        //& 4: shifted ±10n from state 1 or 2

        //* wall mech state transition control
        if(controller.get_digital_new_press(DIGITAL_LEFT)){ //forward one state
            //if(wallMechState < 2){wallMechState += 1;} //! should be 3 when we have the second ring thing
            //else{wallMechState = 0;}
            if(wallMechState == 0){wallMechState = 1; WMIntegral = 0.0;}
            else if(wallMechState == 1){wallMechState = 2; WMIntegral = 0.0;}
            else if(wallMechState == 2){wallMechState = 0; WMIntegral = 0.0;}
            else if(wallMechState == 4){wallMechState = 2; WMIntegral = 0.0;}
            WMIntegral = 0.0;

            if(wallMechState == 1){intakeState = 2;}
            else if(wallMechState == 2){ringYet = 0;}
        }

        /*
        if(controller.get_digital_new_press(DIGITAL_UP) && wallMechState != 0){wallMechState = 4; WMTargetAdjustment += 1;}
        if(controller.get_digital_new_press(DIGITAL_DOWN) && wallMechState != 0){wallMechState = 4; WMTargetAdjustment -= 1;}
        */

        if(controller.get_digital_new_press(DIGITAL_UP)){
            if(wallMechState == 1){WMLoadingTarget += WMAdjustmentIncrement;}
            else if(wallMechState == 2){WMScoringTarget += WMAdjustmentIncrement;}
        }
        if(controller.get_digital_new_press(DIGITAL_DOWN)){
            if(wallMechState == 1){WMLoadingTarget -= WMAdjustmentIncrement;}
            else if(wallMechState == 2){WMScoringTarget -= WMAdjustmentIncrement;}
        }

        //* wall mech state execution control

        if (wallMechState == 0) { wallMechTarget = WMIdleTarget;}
        else if (wallMechState == 1){
            wallMechTarget = WMLoadingTarget;
            if(WMDistanceSensor.get() < WMRingDetectionDist && ringYet == 0){
                ringYet = 1;
                delay(50);
                intake.move_voltage(-intakeVoltage);
                delay(100);
                intake.move_voltage(0.0);
                intakeState = 0;
            }
        }
        //else if (wallMechState == 2) { wallMechTarget = WMLoadingBTarget; }
        else if(wallMechState == 2){wallMechTarget = WMScoringTarget;} //max 440? do we need wall mech calibration 💀 //460 //max 480

        WMPosition = wallMech.get_position();
        WMError = wallMechTarget - WMPosition;
        WMDerivative = WMPreviousPos - WMPosition;
        WMIntegral += WMError;
        if(wallMechTarget < 250.0){WMKi = WMKiForward; WMKd = WMKdForward;}
        else{WMKi = WMKiBackward; WMKd = WMKdBackward;}
        if(wallMechState == 0){WMKi = 0.0;}
        //if (fabs(WMError) >= WMErrorMax){WMIntegral = 0.0;} //not sure if this is ever actually helpful
        if (fabs(WMError) <= WMErrorMin){WMIntegral = 0.0;} //this is to stop the 瑟瑟发抖 //did we ever test this cause still big time 瑟瑟发抖
        if (fabs(WMIntegral * WMKi) >= WMIntegralMax){WMIntegral = getDir(WMIntegral) * WMIntegralMax / WMKi;}
        WMPower = WMKp * WMError + WMKd * WMDerivative + WMKi * WMIntegral;
        WMPreviousPos = WMPosition;

        /*
        //lcd::set_text(4, std::to_string(backClaw.get_value()));
        lcd::set_text(4, std::to_string(WMPower / 1000.0));
        lcd::set_text(5, std::to_string(WMError));
        lcd::set_text(6, std::to_string(WMIntegral * WMKi));
        lcd::set_text(7, std::to_string(wallMech.get_position()));
        */

        //if (fabs(WMError) > 3.0){ wallMech.move_voltage(WMPower); } // If error is over 3 degrees then move
        //else { wallMech.brake(); } // Should be hold type of brake
        wallMech.move_voltage(WMPower);
        //if(controller.get_digital(DIGITAL_B)){wallMech.move_voltage(0.0);}

        delay(10);
}