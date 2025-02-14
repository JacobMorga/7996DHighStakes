#include "main.h"
using namespace pros;

float JRYValue, JRXValue, JLYValue, JLXValue; //joystick right y, right x, left y, left x values

float intakeVoltage = 12000.0; // mV //11000
int exitcode = 0; //color sort loop exitcode
float sortDistance = 20.0; //110.0; //mm
float sortDelay1 = 75.0; //ms
float sortDelay2 = 200.0; //ms
float sortDegrees1 = 200.0; //degrees
float sortDegrees2 = 200.0; //degrees
int intakeStuckCounter = 0;

const float redLimit = 25000.0; //lower limits for rgb sort
const float blueLimit = 17500.0;

float ambient = 0.0; //zero to one
float redQuotient = 0.0;
float blueQuotient = 0.0;

float WMTarget = 0.0; //degrees
float WMError = 0.0; //degrees

float WMKp = 375.0; //450.0;
float WMKd = 350.0; //200.0;
float WMPosition = 0.0; //degrees
float WMPreviousPos = 0.0; //degrees
float WMDerivative = 0.0;
float WMPower = 0.0;
float WMKi = 0.0;
float WMIntegral = 0.0;
float WMIntegralMax = 4000.0; //mV, arbitrary
float WMIdleTarget = 40.0; //degrees
float WMLoadingTarget = 65.0; //degrees
float WMLoadingBTarget = 70.0; //degrees, unused and untested
float WMScoringTarget = 180.0; //degrees
float WMManualSpeed = 0.5; //degrees per cycle
float WMRingDetectionDist = 50.0; //mm

bool colorSorting = 1;
float intakeSort1Start = 0.0;
float intakeSort2Start = 0.0;
float distanceSensed = 0.0;
int comboState = 0;
int sortingState = 0;
int stoppedState = 0;
int reversedState = 0;
int waitingState = 0;
bool dodge = 0;
bool justLoaded = 0;
pros::c::optical_raw_s_t rawColors;
bool prevBackClawBool = 0;
bool backClawBool = 0;

void runDriveCont (){
    //$ Controller mapping:
    //$ Right joystick X axis: Unused
    //$ Right joystick Y axis: Driving
    //$  Left joystick X axis: Turning
    //$  Left joystick Y axis: Unused
    //$                    L1: Wall mech state cycle
    //$                    L2: Back claw toggle
    //$                    R1: Intake toggle                -> Intake whether you have the back claw down
    //$                    R2: Reverse intake, hold button
    //$                    Up: Wall mech manual adjust up   -> Right corner clearer
    //$                  Down: Wall mech manual adjust down -> Intake piston toggle
    //$                  Left: Left corner clearer toggle
    //$                 Right: Color sorting on/off toggle  
    //$                 X (↑): Intake until ring detected   -> Wall mech manual adjust up
    //$                 Y (←): Intake piston toggle         -> Unneeded
    //$                 A (→): Right corner clearer toggle  -> Wall mech manual adjust down
    //$                 B (↓): Team color toggle
    //$ Potential additional adjustments/additions:
    //$ some kind of macro for holding a second ring in the intake and scoring it on the wall stake after the wall mech ring is scored

    while (1){
        JRYValue = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0; // Scales 127 to 100 then cubes and converts to mV
        JLYValue = powf(controller.get_analog(ANALOG_LEFT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JRXValue = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JLXValue = powf(controller.get_analog(ANALOG_LEFT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;

        rightDrive.move_voltage(JRYValue - JLXValue);
        leftDrive.move_voltage(JRYValue + JLXValue);

        if(controller.get_digital_new_press(DIGITAL_L2)){backClaw.set_value(!backClaw.get_value());}
        if(controller.get_digital_new_press(DIGITAL_DOWN)){intakePiston.set_value(!intakePiston.get_value());}
        if(controller.get_digital_new_press(DIGITAL_LEFT)){leftClearer.set_value(!leftClearer.get_value());}
        if(controller.get_digital_new_press(DIGITAL_UP)){rightClearer.set_value(!rightClearer.get_value());}
        if(controller.get_digital_new_press(DIGITAL_RIGHT)){colorSorting = !colorSorting;}
        if(controller.get_digital_new_press(DIGITAL_B)){
            if(teamColor == COLOR_RED){teamColor = COLOR_BLUE;}
            else{teamColor = COLOR_RED;}
        }

        delay(10);
    }
}

//* COLOR CALIBRATION STEPS:
//* 1: Comment combo task in main.cpp >> initialize().
//* 2: Uncomment calibrate function in main.cpp >> opcontrol().
//* 3: Download and run with download cable plugged into brain, brain terminal printing to a new window.
//* 4: Feed ~20 rings of each color through the intake, verifying the output is printing to the terminal.
//*    Note: It might be helpful to feed them through while rotating the robot to face different directions.
//* 5: Copy output into a blank Google Sheet and auto-format the columns, which are redQuotient and blueQuotient.
//* 6: Make graphs of both data sets.
//* 7: Identify peak values that indicate ring detection and ambient lighting value.
//* 8: Input about the midpoint between these values into the variables redLimit and blueLimit above.
//* 9: Run test calibration function in main.cpp >> opcontrol(), ensuring brain terminal output functioning.
//*    Note: Alternatively, you could just drive it around and see if it messes up. 
//*          Printing the diagnosis would be helpful if it does, though.
//* 10: Feed ~20 rings of each color through the intake, verifying correct identification.
//* 11: Repeat calibration if necessary.
//* 12: Comment both run and test calibration functions; uncomment intake task in main.cpp >> initialize().

void runColorCalibration(){
    opticalSensor.set_led_pwm(100.0);
    intake.move_voltage(intakeVoltage);
    while(1){
        rawColors = opticalSensor.get_raw();
        ambient = opticalSensor.get_brightness();
        std::cout << rawColors.red / ambient << ", " << rawColors.blue / ambient << "\n";
        delay(10);
    }
}

void testColorCalibration(){
	float prevDistance = 0.0;
	int ringColor = 0;
	bool colorDecided = 0;
	intake.move_voltage(intakeVoltage);
	opticalSensor.set_led_pwm(100.0);
	while(1){
		rawColors = opticalSensor.get_raw();
		distanceSensed = intakeDistanceSensor.get();
        ambient = opticalSensor.get_brightness();
        redQuotient = rawColors.red / ambient;
        blueQuotient = rawColors.blue / ambient;
		if(distanceSensed < 100){
			if(prevDistance >= 100){std::cout << "ring detected." << "\n";}
			if(colorDecided == 0 && redQuotient >= redLimit){colorDecided = 1; ringColor = 1;}
			if(colorDecided == 0 && blueQuotient >= blueLimit){colorDecided = 1; ringColor = 2;}
			//std::cout << distanceSensed << ", " << rawColors.red << ", " << rawColors.green << ", " << rawColors.blue << ", " << rawColors.clear << "\n";
		}
		else if(distanceSensed >= 100 && prevDistance < 100){
			std::cout << "ring left." << "\n";
			if(colorDecided == 0){std::cout << "color undecided." << "\n" << "\n";}
			else{
				if(ringColor == 1){std::cout << "it was a RED ring." << "\n" << "\n";}
				else if(ringColor == 2){std::cout << "it was a BLUE ring." << "\n" << "\n";}
			}
			colorDecided = 0;
			ringColor = 0;
		}
		prevDistance = distanceSensed;
		delay(10);
	}
}

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
    intakeSort1Start = intakeTop.get_position();
    while(exitcode == 0){
        rawColors = opticalSensor.get_raw();
        ambient = opticalSensor.get_brightness();
        redQuotient = rawColors.red / ambient;
        blueQuotient = rawColors.blue / ambient;
        distanceSensed = intakeDistanceSensor.get();

        if(distanceSensed > sortDistance){exitcode = 1;} //ring passed through color sorter
        else if((redQuotient >= redLimit && teamColor == COLOR_BLUE) || (blueQuotient >= blueLimit && teamColor == COLOR_RED)){exitcode = 2;} //ring flagged color sorting
        else if(controller.get_digital_new_press(DIGITAL_R1)){exitcode = 3;} //exit to corresponding state
        else if(controller.get_digital_new_press(DIGITAL_R2)){exitcode = 4;}
        else if(controller.get_digital_new_press(DIGITAL_A)){exitcode = 5;}
        if(incomingState >= 13 && incomingState <= 15 && ((redQuotient >= redLimit && teamColor == COLOR_RED) || (blueQuotient >= blueLimit && teamColor == COLOR_BLUE))){exitcode = 3;} //pause this ring on intake
        delay(10);
    }
    if(exitcode == 1){comboState = sortingState;} //ring passed color sorting
    else if(exitcode == 2){ //sort flagged ring
        if(dodge){WMTarget = WMIdleTarget;}
        intakeStuckCounter = 0;
        while(intakeDistanceSensor.get() < sortDistance && intakeStuckCounter < 200){delay(10); intakeStuckCounter += 1;} //intended variance decreaser
        intakeStuckCounter = 0;
        while(intakeTop.get_position() < intakeSort1Start + sortDegrees1 && intakeStuckCounter < 200){delay(10); intakeStuckCounter += 1;}    
        intakeStuckCounter = 0;
        intake.move_voltage(-intakeVoltage);
        intakeSort2Start = intakeTop.get_position();
        while(intakeTop.get_position() > intakeSort2Start - sortDegrees2 && intakeStuckCounter < 200){delay(10); intakeStuckCounter += 1;}
        if(dodge){WMTarget = WMLoadingTarget;}
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
    //^ 17: secret state where just the bottom is intaking? unable to use controller to get here though it's only for autons i guess
    while(1){
        //* state transitions
        backClawBool = backClaw.get_value();
        if(controller.get_digital_new_press(DIGITAL_R1)){ //pressed to start intaking 
            if(comboState == 0 || comboState == 2){
                if(backClaw.get_value()){comboState = 1;}
                else{comboState = 3;}
            }
            else if(comboState == 1 || comboState == 3){comboState = 0;}
            else if(comboState == 4){comboState = 5;} 
            else if(comboState == 5 || comboState == 16){comboState = 4;}
            else if(comboState == 6 || comboState == 8){
                if(backClaw.get_value()){comboState = 7;}
                else{comboState = 9;}
            }
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
        else if(backClawBool != prevBackClawBool){
            if(comboState == 1){comboState = 3;}
            else if(comboState == 3){comboState = 1;}
            else if(comboState == 7){comboState = 9;}
            else if(comboState == 9){comboState = 7;}
        }
        else if(WMDistanceSensor.get() <= WMRingDetectionDist && comboState == 4){ //wall mech loaded
            if(comboState == 4){comboState = 5; justLoaded = 1;} //redundant for clarity
        }
        else if(intakeDistanceSensor.get() <= sortDistance && (comboState == 1 || comboState == 3 || comboState == 4 || comboState == 7 || comboState == 9)){ //ring detected
            if(comboState == 1){comboState = 10;}
            else if(comboState == 3){comboState = 13;}
            else if(comboState == 4){comboState = 11;}
            else if(comboState == 7){comboState = 12;}
            else if(comboState == 9){comboState = 15;}
        }
        if(controller.get_digital(DIGITAL_X)){ //manual wall mech target editing
            if(comboState == 4 || comboState == 5){WMLoadingTarget += WMManualSpeed;}
            else if(comboState >= 6 && comboState <= 9){WMScoringTarget -= WMManualSpeed;}
        }
        else if(controller.get_digital(DIGITAL_A)){
            if(comboState == 4 || comboState == 5){WMLoadingTarget -= WMManualSpeed;}
            else if(comboState >= 6 && comboState <= 9){WMScoringTarget += WMManualSpeed;}
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
            WMTarget = WMIdleTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 1){ //^ 1: intaking, wall mech idle (color sort controlled by separate toggle, all still state 1)
            intake.move_voltage(intakeVoltage);
            WMTarget = WMIdleTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 2){ //^ 2: intake reversing, wall mech idle
            intake.move_voltage(-intakeVoltage);
            WMTarget = WMIdleTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 3){ //^ 3: intaking until ring detected, wall mech idle (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            WMTarget = WMIdleTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 4){ //^ 4: intaking, wall mech loading (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            WMTarget = WMLoadingTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 5){ //^ 5: intake stopped, wall mech loaded but down
            if(justLoaded == 1){ //pull hook out of ring
                delay(50);
                intake.move_voltage(-intakeVoltage);
                delay(75);
                justLoaded = 0;
            }
            intake.brake();
            WMTarget = WMLoadingTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 6){ //^ 6: intake stopped, wall mech in scoring position
            intake.brake();
            WMTarget = WMScoringTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 7){ //^ 7: intaking, wall mech scoring (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            WMTarget = WMScoringTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 8){ //^ 8: intake reversing, wall mech scoring
            intake.move_voltage(-intakeVoltage);
            WMTarget = WMScoringTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 9){ //^ 9: intaking until ring detected, wall mech scoring (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            WMTarget = WMScoringTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 10){ //^ 10: intake currently sorting out ring from continuous intake, wall mech idle
            WMTarget = WMIdleTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(10);}
            else{comboState = 1;}
        }
        else if(comboState == 11){ //^ 11: intake currently sorting out ring from continuous intake, wall mech loading
            WMTarget = WMLoadingTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(11);}
            else{comboState = 4;}
        }
        else if(comboState == 12){ //^ 12: intake currently sorting out ring from continuous intake, wall mech scoring
            WMTarget = WMScoringTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(12);}
            else{comboState = 7;}
        }
        else if(comboState == 13){ //^ 13: intake currently sorting out ring from until finding correct ring, wall mech idle
            WMTarget = WMIdleTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(13);}
            else{comboState = 0;}
        }
        else if(comboState == 14){ //^ 14: intake currently sorting out ring from until finding correct ring, wall mech loading
            WMTarget = WMLoadingTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(14);}
            else{comboState = 5;}
        }
        else if(comboState == 15){ //^ 15: intake currently sorting out ring from until finding correct ring, wall mech scoring
            WMTarget = WMScoringTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(15);}
            else{comboState = 6;}
        }
        else if(comboState == 16){ //^ 16: intake reversing, wall mech loading
            intake.move_voltage(-intakeVoltage);
            WMTarget = WMLoadingTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 17){ //^ 17: secret state where just the bottom is intaking? unable to use controller to get here though it's only for autons i guess
            intakeTop.brake();
            intakeBottom.move_voltage(intakeVoltage);
        }
        prevBackClawBool = backClawBool;
        delay(10);
    }
}
float WMDistance;
void runWallMech(){
    while(1){
        WMPosition = WMPotentiometer.get_angle();
        WMError = WMTarget - WMPosition;
        WMIntegral += WMError;
        //if(comboState <= 3 || comboState == 10 || comboState == 13){WMKi = 0.0;}
        //if(getDir(WMIntegral) != getDir(WMError)){WMIntegral = 0.0;} //this is so it turns around faster
        //if (fabs(WMIntegral * WMKi) >= WMIntegralMax){WMIntegral = getDir(WMIntegral) * WMIntegralMax / WMKi;}
        WMDerivative = WMPreviousPos - WMPosition;
        WMPower = WMKp * WMError + WMKi * WMIntegral + WMKd * WMDerivative;
        WMPreviousPos = WMPosition;
        wallMech.move_voltage(WMPower);

        //debug combo system
        //WMDistance = WMDistanceSensor.get();
        /*
        lcd::clear();
        lcd::print(0, "%d : combo state", comboState);
        lcd::print(1, "%d : color sorting", colorSorting);
        if(teamColor == COLOR_RED){lcd::print(2, "RED : team color");}
        else{lcd::print(2, "BLUE : team color");}
        lcd::print(2, "%f : distance of WM sensor", WMDistance);
        lcd::print(3, "%f : position of WM", WMPosition);
        lcd::print(4, "%f : target of WM", WMTarget);
        lcd::print(5, "%f : error of WM", WMError);
        lcd::print(6, "%f : power of WM", WMPower / 1000.0);
        lcd::print(7, "%f : integral power of WM", WMIntegral * WMKi);
        */

        //debug drivetrain speeds
        /*
        lcd::clear();
        lcd::print(0, "%f : right drive speed", (drive1.get_actual_velocity() + drive2.get_actual_velocity() + drive3.get_actual_velocity()) / 3.0);
        lcd::print(1, "%f : left drive speed", (drive4.get_actual_velocity() + drive5.get_actual_velocity() + drive6.get_actual_velocity()) / 3.0);
        lcd::print(2, "%f : drive 1 speed", drive1.get_actual_velocity());
        lcd::print(3, "%f : drive 2 speed", drive2.get_actual_velocity());
        lcd::print(4, "%f : drive 3 speed", drive3.get_actual_velocity());
        lcd::print(5, "%f : drive 4 speed", drive4.get_actual_velocity());
        lcd::print(6, "%f : drive 5 speed", drive5.get_actual_velocity());
        lcd::print(7, "%f : drive 6 speed", drive6.get_actual_velocity());
        */
        
        /*
        lcd::clear();
        lcd::print(0, "%f : xPos", xPos);
        lcd::print(1, "%f : yPos", yPos);
        lcd::print(2, "%f : tPos", tPos);
        lcd::print(4, "%f : power", (20.0 * tPow) / 12000.0 * 600.0);
        lcd::print(5, "%f : error", tError * 180.0 / pi);
        */
        delay(10);
    }
}