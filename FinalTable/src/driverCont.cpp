#include "main.h"
using namespace pros;

float JRYValue, JRXValue, JLYValue, JLXValue; //joystick right y, right x, left y, left x values

int intakeState = 0; //current intake state (0-5)
float intakeVoltage = 12000.0; // mV //11000
int exitcode = 0; //indicator for why color sorting state (4) exited
float sortDistance = 20.0; //110.0; //mm
float sortDelay1 = 75.0; //ms
float sortDelay2 = 200.0; //ms
float sortDegrees1 = 200.0; //degrees
float sortDegrees2 = 300.0; //degrees
int intakeStuckCounter = 0;

const float redLimit = 25000.0; //lower limits for rgbc sort
const float blueLimit = 17500.0;

float ambient = 0.0;
float redQuotient = 0.0;
float blueQuotient = 0.0;

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

float WMKp = 250.0; //100.0; // tuned value for wall mech P-loop
float WMKd = 0.0;
float WMPosition = 0.0;
float WMPreviousPos = 0.0;
float WMDerivative = 0.0;
float WMPower = 0.0;
float WMKi = 1000.0;
float WMIntegral = 0.0;
float WMErrorMax = 200.0;
float WMErrorMin = 1.0;
float WMIntegralMax = 4000.0;
float WMTargetAdjustment = 0.0;
float WMIdleTarget = 30.0; //470.0; //2.0;
float WMLoadingTarget = 55.0; //155.0; //900.0; //130.0;
float WMLoadingBTarget = 180.0; //unused and untested
float WMScoringTarget = 175.0; //445.0; //2800.0; //420.0;
float WMAdjustmentIncrement = 0.5;
float WMRingDetectionDist = 90.0;

bool colorSorting = 1;
float intakeSort1Start = 0.0;
float intakeSort2Start = 0.0;

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
        if(controller.get_digital_new_press(DIGITAL_Y)){intakePiston.set_value(!intakePiston.get_value());} //toggles intake piston
        if(controller.get_digital_new_press(DIGITAL_LEFT)){leftClearer.set_value(!leftClearer.get_value());} //toggles left clearer
        if(controller.get_digital_new_press(DIGITAL_A)){rightClearer.set_value(!rightClearer.get_value());} //toggles right clearer
        if(controller.get_digital_new_press(DIGITAL_RIGHT)){colorSorting = !colorSorting;} //toggles color sorting on/off
        if(controller.get_digital_new_press(DIGITAL_B)){
            if(teamColor == COLOR_RED){teamColor = COLOR_BLUE;}
            else{teamColor = COLOR_RED;}
        }

        delay(10);
    }
}

float distanceSensed = 0.0;
bool ringYet = 0;

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
        //std::cout << distanceMeasured << ", " << ambientBrightness << ", " << rawColors.red << ", " << rawColors.blue << ", " << rgbColors.red << ", " << rgbColors.blue << ", " << measuredHue << ", " << rawColors.red / ambientBrightness << ", " << rawColors.blue / ambientBrightness << ", " << rawColors.red * ambientBrightness << ", " << rawColors.blue * ambientBrightness << ", " << rgbColors.red / ambientBrightness << ", " << rgbColors.blue / ambientBrightness << ", " << rgbColors.red * ambientBrightness << ", " << rgbColors.blue * ambientBrightness << "\n";
        std::cout << rawColors.red / ambientBrightness << ", " << rawColors.blue / ambientBrightness << "\n";
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
    float ambientLight = 0.0;
	while(1){
		rawVals = opticalSensor.get_raw();
		distanceMeasured = distanceSensor.get();
        ambientLight = opticalSensor.get_brightness();
        redQuotient = rawVals.red / ambientLight;
        blueQuotient = rawVals.blue / ambientLight;
		if(distanceMeasured < 100){
			if(prevDistance >= 100){std::cout << "ring detected." << "\n";}
			if(colorDecided == 0 && redQuotient >= redLimit){colorDecided = 1; ringColor = 1;}
			if(colorDecided == 0 && blueQuotient >= blueLimit){colorDecided = 1; ringColor = 2;}
			//std::cout << distanceMeasured << ", " << rawVals.red << ", " << rawVals.green << ", " << rawVals.blue << ", " << rawVals.clear << "\n";
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

int comboState = 0;
float comboStateDecimal = 0.0;
int sortingState = 0;
int stoppedState = 0;
int reversedState = 0;
int waitingState = 0;
bool dodge = 0;

bool justLoaded = 0;

void colorSort(int incomingState){
    if(incomingState == 10 || incomingState == 13){
        sortingState = 1;
        stoppedState = 0;
        reversedState = 2;
        waitingState = 3;
        dodge = 0;
    }
    else if(incomingState == 11 || incomingState == 14){
        sortingState = 4;
        stoppedState = 6;
        reversedState = 16;
        waitingState = 3;
        dodge = 1;
    }
    else if(incomingState == 12 || incomingState == 15){
        sortingState = 7;
        stoppedState = 6;
        reversedState = 8;
        waitingState = 9;
        dodge = 0;
    }

    opticalSensor.set_led_pwm(100.0);
    exitcode = 0;
    while(exitcode == 0){
        rawColors = opticalSensor.get_raw();
        ambient = opticalSensor.get_brightness();
        redQuotient = rawColors.red / ambient;
        blueQuotient = rawColors.blue / ambient;
        distanceSensed = distanceSensor.get();

        if(distanceSensed > sortDistance){exitcode = 1;} //ring passed color sorting
        else if((redQuotient >= redLimit && teamColor == COLOR_BLUE) || (blueQuotient >= blueLimit && teamColor == COLOR_RED)){exitcode = 2;} //ring flagged color sorting
        else if(controller.get_digital(DIGITAL_R1)){exitcode = 3;} //exit to corresponding state
        else if(controller.get_digital(DIGITAL_R2)){exitcode = 4;}
        else if(controller.get_digital(DIGITAL_A)){exitcode = 5;}
        if(incomingState >= 13 && incomingState <= 15 && ((redQuotient >= redLimit && teamColor == COLOR_RED) || (blueQuotient >= blueLimit && teamColor == COLOR_BLUE))){exitcode = 3;}
        delay(10);
    }
    if(exitcode == 1){comboState = sortingState;} //ring passed color sorting
    else if(exitcode == 2){ //sort flagged ring
        if(dodge){wallMechTarget = WMIdleTarget;}
        intakeSort1Start = intakeTop.get_position();
        intakeStuckCounter = 0;
        while(distanceSensor.get() < sortDistance && intakeStuckCounter < 200){delay(10); intakeStuckCounter += 1;} //intended variance decreaser
        intakeStuckCounter = 0;
        while(intakeTop.get_position() < intakeSort1Start + sortDegrees1 && intakeStuckCounter < 200){delay(10); intakeStuckCounter += 1;}    
        intakeStuckCounter = 0;
        intake.move_voltage(-intakeVoltage);
        intakeSort2Start = intakeTop.get_position();
        while(intakeTop.get_position() > intakeSort2Start - sortDegrees2 && intakeStuckCounter < 200){delay(10); intakeStuckCounter += 1;}
        if(dodge){wallMechTarget = WMLoadingTarget;}
        if(incomingState >= 13 && incomingState <= 15){comboState = waitingState;}
        else{comboState = sortingState;}
    }
    else if(exitcode == 3){comboState = stoppedState;} //manual or automatic stop
    else if(exitcode == 4){comboState = reversedState;} //manual reverse
    else if(exitcode == 5){comboState = waitingState;} //manual wait until ring
}

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
    //^ 10: intake currently sorting out ring from continuous intake, wall mech idle
    //^ 11: intake currently sorting out ring from continuous intake, wall mech loading
    //^ 12: intake currently sorting out ring from continuous intake, wall mech scoring
    //^ 13: intake currently sorting out ring from until finding correct ring, wall mech idle
    //^ 14: intake currently sorting out ring from until finding correct ring, wall mech loading
    //^ 15: intake currently sorting out ring from until finding correct ring, wall mech scoring
    //^ 16: intake reversing, wall mech loading
    while(1){
        //* state transitions
        if(controller.get_digital_new_press(DIGITAL_R1)){ //pressed to start intaking 
            if(comboState == 0 || comboState == 2){comboState = 1;}
            else if(comboState == 1 || comboState == 3){comboState = 0;}
            else if(comboState == 4){comboState = 5;} 
            else if(comboState == 5 || comboState == 16){comboState = 4;}
            else if(comboState == 6 || comboState == 8){comboState = 7;}
            else if(comboState == 7 || comboState == 9){comboState = 6;}
        }
        else if(controller.get_digital_new_press(DIGITAL_L1)){ //pressed to cycle wall mech 
            if(comboState <= 3){comboState = 4;}
            else if(comboState == 4){comboState = 7;}
            else if(comboState == 5){comboState = 6;}
            else if(comboState == 6){comboState = 0;}
            else if(comboState == 7){comboState = 1;}
            else if(comboState == 8){comboState = 2;}
            else if(comboState == 9){comboState = 3;}
            else if(comboState == 16){comboState = 8;}
        }
        else if(controller.get_digital_new_press(DIGITAL_X)){ //intake until ring seen
            if(comboState == 3){comboState = 0;}
            else if(comboState <= 5 || comboState == 16){comboState = 3;}
            else if(comboState >= 6 && comboState <= 8){comboState = 9;}
            else if(comboState == 9){comboState = 6;}        
        }
        else if(WMDistanceSensor.get() <= WMRingDetectionDist){ //wall mech loaded
            if(comboState == 4){comboState = 5; justLoaded = 1;}
        }
        else if(distanceSensor.get() <= sortDistance){ //ring detected
            if(comboState == 1){comboState = 10;}
            else if(comboState == 3){comboState = 13;}
            else if(comboState == 4){comboState = 11;}
            else if(comboState == 7){comboState = 12;}
            else if(comboState == 9){comboState = 15;}
        }
        else if(controller.get_digital(DIGITAL_UP)){ //manual wall mech target editing
            if(comboState == 4 || comboState == 5){WMLoadingTarget += WMAdjustmentIncrement;}
            else if(comboState >= 6 && comboState <= 9){WMScoringTarget += WMAdjustmentIncrement;}
        }
        else if(controller.get_digital(DIGITAL_DOWN)){
            if(comboState == 4 || comboState == 5){WMLoadingTarget -= WMAdjustmentIncrement;}
            else if(comboState >= 6 && comboState <= 9){WMScoringTarget -= WMAdjustmentIncrement;}
        }
        else if(controller.get_digital(DIGITAL_R2)){ //intake reverse button 
            if(comboState <= 3){comboState = 2;}
            else if(comboState == 4 || comboState == 5){comboState = 16;}
            else if(comboState >= 6 && comboState <= 9){comboState = 8;}
        }
        else if(controller.get_digital(DIGITAL_R2) == 0){ //let go of the reverse button 
            if(comboState == 2){comboState = 0;}
            else if(comboState == 8){comboState = 6;}
            else if(comboState == 16){comboState = 5;}
        }

        //* state executions
        if(comboState == 0){ //^ 0: intake off, wall mech idle
            intake.brake();
            wallMechTarget = WMIdleTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 1){ //^ 1: intaking, wall mech idle (color sort controlled by separate toggle, all still state 1)
            intake.move_voltage(intakeVoltage);
            wallMechTarget = WMIdleTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 2){ //^ 2: intake reversing, wall mech idle
            intake.move_voltage(-intakeVoltage);
            wallMechTarget = WMIdleTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 3){ //^ 3: intaking until ring detected, wall mech idle (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            wallMechTarget = WMIdleTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 4){ //^ 4: intaking, wall mech loading (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            wallMechTarget = WMLoadingTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 5){ //^ 5: intake stopped, wall mech loaded but down
            if(justLoaded == 1){
                //delay(25);
                intake.move_voltage(-intakeVoltage);
                delay(75);
                justLoaded = 0;
            }
            intake.brake();
            wallMechTarget = WMLoadingTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 6){ //^ 6: intake stopped, wall mech in scoring position
            intake.brake();
            wallMechTarget = WMScoringTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 7){ //^ 7: intaking, wall mech scoring (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            wallMechTarget = WMScoringTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 8){ //^ 8: intake reversing, wall mech scoring
            intake.move_voltage(-intakeVoltage);
            wallMechTarget = WMScoringTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 9){ //^ 9: intaking until ring detected, wall mech scoring (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            wallMechTarget = WMScoringTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 10){ //^ 10: intake currently sorting out ring from continuous intake, wall mech idle
            wallMechTarget = WMIdleTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(10);}
            else{comboState = 1;}
        }
        else if(comboState == 11){ //^ 11: intake currently sorting out ring from continuous intake, wall mech loading
            wallMechTarget = WMLoadingTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(11);}
            else{comboState = 4;}
        }
        else if(comboState == 12){ //^ 12: intake currently sorting out ring from continuous intake, wall mech scoring
            wallMechTarget = WMScoringTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(12);}
            else{comboState = 7;}
        }
        else if(comboState == 13){ //^ 13: intake currently sorting out ring from until finding correct ring, wall mech idle
            wallMechTarget = WMIdleTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(13);}
            else{comboState = 0;}
        }
        else if(comboState == 14){ //^ 14: intake currently sorting out ring from until finding correct ring, wall mech loading
            wallMechTarget = WMLoadingTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(14);}
            else{comboState = 5;}
        }
        else if(comboState == 15){ //^ 15: intake currently sorting out ring from until finding correct ring, wall mech scoring
            wallMechTarget = WMScoringTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(15);}
            else{comboState = 6;}
        }
        else if(comboState == 16){ //^ 16: intake reversing, wall mech loading
            intake.move_voltage(-intakeVoltage);
            wallMechTarget = WMLoadingTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        delay(10);
    }
}

void wallMechRunning(){
    while(1){
        WMPosition = wallMechPotentiometer.get_angle();
        WMError = wallMechTarget - WMPosition;
        WMDerivative = WMPreviousPos - WMPosition;
        WMIntegral += WMError;
        if(comboState <= 3 || comboState == 10 || comboState == 13){WMKi = 0.0;}
        if(getDir(WMIntegral) != getDir(WMError)){WMIntegral = 0.0;} //this is so it corrects back faster
        if (fabs(WMIntegral * WMKi) >= WMIntegralMax){WMIntegral = getDir(WMIntegral) * WMIntegralMax / WMKi;}
        WMPower = WMKp * WMError + WMKd * WMDerivative + WMKi * WMIntegral;
        WMPreviousPos = WMPosition;
        wallMech.move_voltage(WMPower);

        lcd::set_text(0, std::to_string(comboState));
        /*
        lcd::set_text(1, std::to_string(colorSorting));
        lcd::set_text(2, std::to_string(teamColor));
        lcd::set_text(3, std::to_string(intakeTop.get_position()));
        lcd::set_text(4, std::to_string(opticalSensor.get_raw().red / opticalSensor.get_brightness()));
        lcd::set_text(5, std::to_string(opticalSensor.get_raw().red));
        lcd::set_text(6, std::to_string(opticalSensor.get_brightness()));
        lcd::set_text(7, std::to_string(intakeStuckCounter));
        */

        lcd::set_text(3, std::to_string(WMPosition));
        lcd::set_text(4, std::to_string(wallMechTarget));
        lcd::set_text(5, std::to_string(WMError));
        lcd::set_text(6, std::to_string(WMPower / 1000.0));
        lcd::set_text(7, std::to_string(WMIntegral * WMKi));

        delay(10);
    }
}