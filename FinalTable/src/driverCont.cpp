#include "main.h"
using namespace pros;

float JRYValue, JRXValue, JLYValue, JLXValue; //joystick right y, right x, left y, left x values

float intakeVoltage = 12000.0; // mV //11000
int exitcode = 0; //color sort loop exitcode
float sortDistance = 20.0; //110.0; //mm
float opticalSortProximity = 200.0; //whatever proximity unit ts returns //& this was 70 before last day if the color sorting starts funching really hard
//float sortDelay1 = 750.0; //ms
//float sortDelay2 = 200.0; //ms
float sortDegrees1 = 150.0; //!130 degrees
float sortDegrees2 = 400.0; //degrees
float stickItIn = 500.0; //ms //!250 //maybe the wall mech spacing that we changed the night before is a little too tight
float pullItOut = 75.0; //ms
int intakeStuckCounter = 0;

const float redLimit = 6000.0; //22000.0; //lower limits for rgb sort
const float blueLimit = 4250.0; //17500.0;
const float backClawDisLimit = 60.0;

float ambient = 0.0; //zero to one
float redQuotient = 0.0;
float blueQuotient = 0.0;

float WMTarget = 0.0; //degrees
float WMError = 0.0; //degrees

float WMKp = 27.5; //300.0; //300.0; 
float WMKi = 0.0; //0.1 //5.0; //5.0;
float WMKd = 125.0; // 50.0 //1000.0; //350.0; 
float WallMechAntiGravTuner = 2.0;
float WMIntegralMax = 4000.0; //mV, arbitrary
float WMErrorMax = 20.0;

float WMIntegral = 0.0;
float WMPosition = 0.0; //degrees
float WMPreviousPos = 0.0; //degrees
float WMDerivative = 0.0;
float WMPower = 0.0;

float WMIdleTarget = 300.0; //20.0; //35.0; //degrees
float WMLoadingTarget = 675.0; //degrees //!60
float WMLoadingBTarget = 70.0; //degrees, unused and untested
float WMScoringTarget = 3000.0; //200.0; //120.0; //164.0; //!175.0; 
float WMForwardTarget = 2000.0; //sets to 1200 at the start of driver
float WMHangTarget = 1000.0;
float WMDescoreTarget = 2275.0; //oh no 

float WMManualSpeed = 20; //tiny potentiometer units per cycle
float WMRingDetectionDist = 45.0; //94.0; //mm //!a little close, no?
float prevWMTarget = 0.0;

bool colorSorting = 1;
float intakeSort1Start = 0.0;
float intakeSort2Start = 0.0;
float distanceSensed = 0.0;
float opticalProximity = 0.0;
int comboState = 0;
int prevComboState = 0;
int sortingState = 0;
int stoppedState = 0;
int reversedState = 0;
int waitingState = 0;
bool dodge = 0;
bool justLoaded = 0;
pros::c::optical_raw_s_t rawColors;
bool prevBackClawBool = 0;
bool backClawBool = 0;
 
bool instantLift = 0;
bool instantLift2 = 0;
bool forcedTransit = 0;
int forcedState = 0;
bool specialIntake = 0;
bool driverControlBool = 0;

bool backClawDisTrigger1 = 0;
bool backClawDisTrigger2 = 0;
bool autoClampBlocked = 0;
bool checkDejam = 0;
int dejamIncomingState;
float dejamBarrier = 25.0;
int dejamtimer = 0;
int dejamtimerlimit = 20;
float hangSlowing = 1.0;
bool pitchSprint = 0;
//bool state21stop = 1;
bool justLifted = 0;

bool printTeamBool = 1;
bool allowDebug = 1;

void funch(){
    lcd::set_text(0, std::to_string('funch'));
}

#define intakeTog        DIGITAL_R1
#define intakeRev        DIGITAL_R2
#define wallMechCycle    DIGITAL_L1
#define backClawTog      DIGITAL_L2
#define colorSortingTog  DIGITAL_B
#define wallMechUp       DIGITAL_X
#define wallMechDown     DIGITAL_A
#define leftCC           DIGITAL_LEFT
#define rightCC          DIGITAL_UP
#define teamColorTog     DIGITAL_RIGHT
#define intakePis        DIGITAL_DOWN
#define instantLiftTog   DIGITAL_Y
#define specialIntakeTog DIGITAL_LEFT
#define engageHang       DIGITAL_DOWN

void runDriveCont (){
    //$ Controller mapping:
    //$ Right joystick X axis: -------
    //$ Right joystick Y axis: Driving
    //$  Left joystick X axis: Turning
    //$  Left joystick Y axis: -------

    //$                    L1: Wall mech state cycle
    //$                    L2: Back claw toggle
    //$                    R1: Intake toggle
    //$                    R2: Reverse intake, hold button

    //$                    Up: Right corner clearer toggle
    //$                  Down: Hang toggle
    //$                  Left: Left corner clearer toggle
    //$                 Right: Debug: Team color switch
    
    //$                 X (↑): Debug: Manual wall mech driving up/in/+ in goal tipping state
    //$                 Y (←): Debug: Instant lift toggle
    //$                 A (→): Debug: Manual wall mech driving down/out/- in goal tipping state
    //$                 B (↓): Color sorting on/off toggle

    instantLift = 0;
    instantLift2 = 0;
    specialIntake = 0;
    driverControlBool = 1;
    WMForwardTarget = 1200.0;
    WMScoringTarget = 3000.0;
    comboState = 0;
    forcedTransit = 0;
    forcedState = 0;
    //backClaw.set_value(1);
    while (1){
        JRYValue = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0; // Scales 127 to 100 then cubes and converts to mV
        JLYValue = powf(controller.get_analog(ANALOG_LEFT_Y) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JRXValue = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;
        JLXValue = powf(controller.get_analog(ANALOG_LEFT_X) / 127.0 * 100.0, 3.0) / 1000.0 * 12.0;

        if(comboState != 23){hangSlowing = 1.0;}
        else{
            if(backClawBool){hangSlowing = 0.67;}
            else{hangSlowing = 0.5;}
        }

        if(inertial1.get_pitch() >= -10.0 || comboState != 23){pitchSprint = 0;}
        else{pitchSprint = 1;}

        if(pitchSprint != 1){
            rightDrive.move_voltage(hangSlowing * (JRYValue - JLXValue));
            leftDrive.move_voltage(hangSlowing * (JRYValue + JLXValue));
        }
        else{drivetrain.move_voltage(12000.0);}
        
        //if(controller.get_digital_new_press(intakePis)){intakePiston.set_value(!intakePiston.get_value());}
        if(controller.get_digital_new_press(leftCC)){leftClearer.set_value(!leftClearer.get_value());}
        if(controller.get_digital_new_press(rightCC)){rightClearer.set_value(!rightClearer.get_value());}
        if(controller.get_digital_new_press(colorSortingTog)){colorSorting = !colorSorting;}
        if(allowDebug){
            if(controller.get_digital_new_press(teamColorTog)){
                if(teamColor == COLOR_RED){teamColor = COLOR_BLUE;}
                else{teamColor = COLOR_RED;}
            }
            if(controller.get_digital_new_press(instantLiftTog)){instantLift2 = !instantLift2;}
            //if(controller.get_digital_new_press(specialIntakeTog)){specialIntake = !specialIntake;}
        }

        //lcd::set_text(1, std::to_string(WMPotentiometer.get_value()));

        delay(10);

        //wallMech.move_voltage(12000.0 * (controller.get_digital(DIGITAL_L1) - controller.get_digital(DIGITAL_L2)));
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

vector<string> ColorCalList = {};
float highColorVal = 0.0;
float lowColorVal = 100000000.0;
void runColorCalibration(){
    cout << "doing it" << "\n";
    opticalSensor.set_led_pwm(100.0);
    while(controller.get_digital(DIGITAL_A) == 0){//for(int silk =0; silk < 1000; silk++){
        intake.move_voltage(intakeVoltage);
        rawColors = opticalSensor.get_raw();
        ambient = opticalSensor.get_brightness();
        if(rawColors.blue / ambient > highColorVal){highColorVal = rawColors.blue / ambient;}
        if(rawColors.blue / ambient < lowColorVal){lowColorVal = rawColors.blue / ambient;}

        ColorCalList.push_back(to_string(rawColors.red / ambient) + ", " + to_string(rawColors.blue / ambient));
        delay(10);
    }
    intake.brake();
    delay(1000);
    lcd::set_text(0, std::to_string(highColorVal));
    lcd::set_text(1, std::to_string(lowColorVal));
    lcd::set_text(2, std::to_string(0.5 * (highColorVal + lowColorVal)));
    intake.move_voltage(0);
    
    for (string item : ColorCalList){

        cout << item << "\n";
        delay(1);   
    }
    delay(1000000000);
}

void testColorCalibration(){
	float prevDistance = 0.0;
	float prevProximity = 0.0;
	int ringColor = 0;
	bool colorDecided = 0;
	intake.move_voltage(intakeVoltage);
	opticalSensor.set_led_pwm(100.0);
	while(1){
		rawColors = opticalSensor.get_raw();
		//distanceSensed = WMDistanceSensor.get();
        opticalProximity = opticalSensor.get_proximity();
        ambient = opticalSensor.get_brightness();
        redQuotient = rawColors.red / ambient;
        blueQuotient = rawColors.blue / ambient;
		if(opticalProximity > opticalSortProximity){
			if(prevDistance >= 100){std::cout << "ring detected." << "\n";}
			if(colorDecided == 0 && redQuotient >= redLimit){colorDecided = 1; ringColor = 1;}
			if(colorDecided == 0 && blueQuotient >= blueLimit){colorDecided = 1; ringColor = 2;}
			//std::cout << distanceSensed << ", " << rawColors.red << ", " << rawColors.green << ", " << rawColors.blue << ", " << rawColors.clear << "\n";
		}
		else if(opticalProximity < opticalSortProximity && prevProximity > opticalSortProximity){
			std::cout << "ring left." << "\n";
			if(colorDecided == 0){std::cout << "color undecided." << "\n" << "\n";}
			else{
				if(ringColor == 1){std::cout << "it was a RED ring." << "\n" << "\n";}
				else if(ringColor == 2){std::cout << "it was a BLUE ring." << "\n" << "\n";}
			}
			colorDecided = 0;
			ringColor = 0;
		}
		//prevDistance = distanceSensed;
        prevProximity = opticalProximity;
		delay(10);
	}
}

bool dejamInColorSort = 0;

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
    else if(incomingState == 24 || incomingState == 27){
        sortingState = 21;
        stoppedState = 22;
        reversedState = 25;
        waitingState = 26;
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
        //distanceSensed = WMDistanceSensor.get();
        opticalProximity = opticalSensor.get_proximity();
        //distanceSensed = intakeDistanceSensor.get();

        if(opticalProximity < opticalSortProximity){exitcode = 1;} //ring passed through color sorter
        else if((redQuotient >= redLimit && teamColor == COLOR_BLUE) || (blueQuotient >= blueLimit && teamColor == COLOR_RED)){exitcode = 2;} //ring flagged color sorting
        else if(controller.get_digital_new_press(intakeTog)){exitcode = 3;} //exit to corresponding state
        else if(controller.get_digital_new_press(intakeRev)){exitcode = 4;}
        //else if(controller.get_digital_new_press(DIGITAL_A)){exitcode = 5;} //? don't think we need this anymore
        else if(forcedTransit == 1){exitcode = 6; forcedTransit = 0;}
        if(((incomingState >= 13 && incomingState <= 15) || incomingState == 27) && ((redQuotient >= redLimit && teamColor == COLOR_RED) || (blueQuotient >= blueLimit && teamColor == COLOR_BLUE))){exitcode = 3;} //pause this ring on intake
        delay(10);
    }
    
    if(exitcode == 1){comboState = sortingState;} //ring passed color sorting
    else if(exitcode == 2){ //sort flagged ring
        if(dodge){WMTarget = WMIdleTarget;}

        /*
        intakeStuckCounter = 0;
        while(WMDistanceSensor.get() < sortDistance && intakeStuckCounter < 200){ //!&& forcedTransit == 0
            intakeStuckCounter += 1;
            delay(10);
        }
        funch();
        */

        intakeStuckCounter = 0;
        while(intakeDistanceSensor.get() >= 100.0 && dejamInColorSort == 0){
            if(intakeTop.get_actual_velocity() <= dejamBarrier){dejamInColorSort = 1;}
            delay(10);
        }
        if(dejamInColorSort){intakeTop.move_voltage(-intakeVoltage); delay(200); dejamInColorSort = 0;}

        intakeStuckCounter = 0;
        intakeSort1Start = intakeTop.get_position(); //funch
        while(intakeTop.get_position() < intakeSort1Start + sortDegrees1 && dejamInColorSort == 0){ //&& forcedTransit == 0
            if(intakeTop.get_actual_velocity() <= dejamBarrier){dejamInColorSort = 1;}
            delay(10);
        }
        if(dejamInColorSort){intakeTop.move_voltage(-intakeVoltage); delay(200); dejamInColorSort = 0;}

        intake.move_voltage(-intakeVoltage);
        intakeStuckCounter = 0;
        intakeSort2Start = intakeTop.get_position(); //funch
        while(intakeTop.get_position() > intakeSort2Start - sortDegrees2){ //&& forcedTransit == 0
            delay(10);
        }

        if(dodge){WMTarget = WMLoadingTarget;}
        if((incomingState >= 13 && incomingState <= 15) || incomingState == 27){comboState = waitingState;}
        else{comboState = sortingState;}
        if(forcedTransit == 1){comboState = forcedState; forcedTransit = 0;}
    }
    else if(exitcode == 3){comboState = stoppedState;} //manual or automatic stop
    else if(exitcode == 4){comboState = reversedState;} //manual reverse
    else if(exitcode == 5){comboState = waitingState;} //manual wait until ring
    else if(exitcode == 6){comboState = forcedState;} //force exit of color sort in auton
    checkDejam = 0;
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
    //^ 18: second secret state where the bottom's outtaking while the top's intaking. controller unable to reach this state
    //^ 19: third secret state where its 17 but the wall mech is up
    //^ 20: dejam
    //^ 21: intake running, wall mech out forward
    //^ 22: intake stopped, wall mech out forward
    //^ 23: hang state (intake stopped, wall mech barely high enough to get the hang bars up)
    //^ 24: color sort out of forward position continuous
    //^ 25: reverse while wall mech forward
    //^ 26: intake running until finding correct ring, wall mech forward
    //^ 27: color sort out of forward position until finding correct ring

    //%  BABE WAKE UP NEW STATE MACHINE JUST DROPPED
    //%  ##: wall mech, intake.
    //%  00: idle, off.
    //%         R1 -> 01
    //%         R2 -> 02
    //%         L1 -> 11
    //%  01: idle, running.
    //%         R1 -> 00
    //%         R2 -> 02
    //%         L1 -> 11
    //%         D1 -> 100
    //%  02: idle, reversing.
    //%         R1 -> 01
    //%         R2 released -> 00
    //%         L1 -> 11
    //%  10: loading 1, off.
    //%         R1 -> 11
    //%         R2 -> 12
    //%         L1 -> 30
    //%  11: loading 1, running.
    //%         R1 -> 10
    //%         R2 -> 12
    //%         L1 -> 31
    //%         D1 -> 100
    //%         D2 -> 21
    //%  12: loading 1, reversing.
    //%         R1 -> 11
    //%         R2 released -> 10
    //%         L1 -> 32
    //%  20: loading 2, off.
    //%         R1 -> 00
    //%         R2 -> 02
    //%         L1 -> 11
    //%  21: loading 2, running.
    //%         R1 -> 20
    //%         R2 -> 22
    //%         L1 -> 31
    //%         D1 -> 100
    //%         D2 -> 20
    //%  22: loading 2, reversing.
    //%         R1 -> 21
    //%         R2 released -> 20
    //%         L1 -> 32
    //%  30: scoring, off.
    //%         R1 -> 31
    //%         R2 -> 32
    //%         L1 -> 00
    //%  31: scoring, running.
    //%         R1 -> 30
    //%         R2 -> 32
    //%         L1 -> 01
    //%         D1 -> 100
    //%  32: scoring, reversing.
    //%         R1 -> 31
    //%         R2 released -> 30
    //%         L1 -> 02
    //%  40: manual messing about, off.
    //%  41: manual messing about, running.
    //%  42: manual messing about, reversing.
    //% 100: sorting

    while(1){
        //* state transitions
        backClawBool = backClaw.get_value();
        if(controller.get_digital_new_press(intakeTog)){ //pressed to start intaking 
            if(comboState == 0 || comboState == 2 || comboState == 23){
                if(backClawBool){comboState = 1;}
                else{comboState = 3;}
            }
            else if(comboState == 1 || comboState == 3){comboState = 0;}
            else if(comboState == 4){comboState = 5;}
            else if(comboState == 5 || comboState == 16){comboState = 4;}
            else if(comboState == 6 || comboState == 8){
                if(backClawBool){comboState = 7;}
                else{comboState = 9;}
            }
            else if(comboState == 7 || comboState == 9){comboState = 6;}
            else if(comboState == 21 || comboState == 26){comboState = 22;}
            else if(comboState == 22 || comboState == 25){
                if(backClawBool){comboState = 21;}
                else{comboState = 26;}
            }
        }
        else if(controller.get_digital_new_press(wallMechCycle)){ //pressed to cycle wall mech 
            if(comboState <= 3){comboState = 4;}
            else if(comboState == 4 || comboState == 21 || comboState == 26){
                if(backClawBool){comboState = 7;}
                else{comboState = 9;}
            }
            else if(comboState == 5 || comboState == 22){comboState = 6;}
            else if(comboState == 6 || comboState == 23){comboState = 0;}
            else if(comboState == 7){comboState = 1;}
            else if(comboState == 8){comboState = 2;}
            else if(comboState == 9){comboState = 3;}
            else if(comboState == 16){comboState = 8;}
        }
        else if(controller.get_digital_new_press(engageHang)){ //pressed to toggle hang
            if(comboState != 23){comboState = 23;}
            else{comboState = 0;}
        }
        else if(backClawBool != prevBackClawBool){ //when back claw open/closes
            if(comboState == 1 && backClawBool == 0){comboState = 3;}
            else if(comboState == 3 && backClawBool == 1){comboState = 1;}
            else if(comboState == 7 && backClawBool == 0){comboState = 9;}
            else if(comboState == 9 && backClawBool == 1){comboState = 7;}
            else if(comboState == 21 && backClawBool == 0){comboState = 26;}
            else if(comboState == 26 && backClawBool == 1){comboState = 21;}
        }
        else if(intakeDistanceSensor.get() <= WMRingDetectionDist && comboState == 4){ //wall mech loaded
            if(comboState == 4 && fabs(WMLoadingTarget - WMPosition) <= 50.0){ //redundant for clarity //!was <=5.0 but we changed units so idk what it should be now
                /*
                if(instantLift){
                    if(specialIntake){comboState = 19;}
                    else{comboState = 6;}
                }
                else if(instantLift2){
                    comboState = 22;
                }
                else{comboState = 5;}
                */
                comboState = 5;
                justLoaded = 1;
            }
        }
        else if(opticalSensor.get_proximity() >= opticalSortProximity && (comboState == 1 || comboState == 3 || comboState == 4 || comboState == 7 || comboState == 9 || comboState == 21 || comboState == 26)){ //ring detected
            if(comboState == 1){comboState = 10;}
            else if(comboState == 3){
                if(!specialIntake){comboState = 13;}
                else{comboState = 17;}
            }
            else if(comboState == 4){comboState = 11;}
            else if(comboState == 7){comboState = 12;}
            else if(comboState == 9){comboState = 15;}
            else if(comboState == 21){comboState = 24;}
            else if(comboState == 26){comboState = 27;}
        }
        if((fabs(intakeTop.get_actual_velocity()) <= dejamBarrier) && ((comboState >= 1 && comboState <= 4) || (comboState >= 7 && comboState <= 16)) && checkDejam){ //if intake jamming
            dejamIncomingState = comboState;
            comboState = 20;
        }
        if(controller.get_digital_new_press(wallMechUp) && allowDebug){ //manual wall mech target editing
            if(comboState == 4 || comboState == 5){WMLoadingTarget += WMManualSpeed;}
            else if(comboState >= 6 && comboState <= 9){WMScoringTarget -= WMManualSpeed;} // + or - up to driver
        }
        else if(controller.get_digital_new_press(wallMechDown) && allowDebug){
            if(comboState == 4 || comboState == 5){WMLoadingTarget -= WMManualSpeed;}
            else if(comboState >= 6 && comboState <= 9){WMScoringTarget += WMManualSpeed;} // + or - up to driver
        }
        else if(controller.get_digital(intakeRev)){ //intake reverse button 
            if(comboState <= 3 || comboState == 23){comboState = 2;}
            else if(comboState == 4 || comboState == 5){comboState = 16;}
            else if(comboState >= 6 && comboState <= 9){comboState = 8;}
            else if(comboState >= 21 && comboState <= 27 && comboState != 23){comboState = 25;}
        }
        else if(controller.get_digital(intakeRev) == 0 && driverControlBool == 1){ //let go of the reverse button 
            if(comboState == 2){comboState = 0;}
            else if(comboState == 8){comboState = 6;}
            else if(comboState == 16){comboState = 5;}
            else if(comboState == 25){comboState = 22;}
        }

        if(comboState != prevComboState && comboState != 20){checkDejam = 0; dejamtimer = 0;}
        prevComboState = comboState;

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
            if(checkDejam == 0 && intakeTop.get_actual_velocity() >= 200.0){checkDejam = 1;}
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
            if(checkDejam == 0 && intakeTop.get_actual_velocity() >= 200.0){checkDejam = 1;}
        }
        else if(comboState == 4){ //^ 4: intaking, wall mech loading (color sort by toggle)
            intake.move_voltage(intakeVoltage);
            WMTarget = WMLoadingTarget;
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
            if(checkDejam == 0 && intakeTop.get_actual_velocity() >= 200.0){checkDejam = 1;}
        }
        else if(comboState == 5){ //^ 5: intake stopped, wall mech loaded but down
            if(justLoaded == 1){ //pull hook out of ring
                delay(stickItIn);
                intake.move_voltage(-intakeVoltage);
                delay(pullItOut);
                justLoaded = 0;
                if(instantLift){
                    if(specialIntake){comboState = 19;}
                    else{comboState = 6;}
                }
                else if(instantLift2){
                    if(backClawBool){comboState = 21;}
                    else{comboState = 26;}
                    justLifted = 1;
                }
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
            if(justLoaded == 1){ //pull hook out of ring
                delay(stickItIn);
                intake.move_voltage(-intakeVoltage);
                WMTarget = WMScoringTarget;
                delay(pullItOut);
                justLoaded = 0;
            }
            WMTarget = WMScoringTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
            if(checkDejam == 0 && intakeTop.get_actual_velocity() >= 200.0){checkDejam = 1;}
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
            if(checkDejam == 0 && intakeTop.get_actual_velocity() >= 200.0){checkDejam = 1;}
        }
        else if(comboState == 10){ //^ 10: intake currently sorting out ring from continuous intake, wall mech idle
            WMTarget = WMIdleTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(10);}
            else{comboState = 1;}
            prevComboState = 1;
        }
        else if(comboState == 11){ //^ 11: intake currently sorting out ring from continuous intake, wall mech loading
            WMTarget = WMLoadingTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(11);}
            else{comboState = 4;}
            prevComboState = 4;
        }
        else if(comboState == 12){ //^ 12: intake currently sorting out ring from continuous intake, wall mech scoring
            WMTarget = WMScoringTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(12);}
            else{comboState = 7;}
            prevComboState = 7;
        }
        else if(comboState == 13){ //^ 13: intake currently sorting out ring from until finding correct ring, wall mech idle
            WMTarget = WMIdleTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(13);}
            else{comboState = 0;}
            prevComboState = 3;
        }
        else if(comboState == 14){ //^ 14: intake currently sorting out ring from until finding correct ring, wall mech loading
            WMTarget = WMLoadingTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(14);}
            else{comboState = 5;}
            prevComboState = 5;
        }
        else if(comboState == 15){ //^ 15: intake currently sorting out ring from until finding correct ring, wall mech scoring
            WMTarget = WMScoringTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(15);}
            else{comboState = 6;}
            prevComboState = 6;
        }
        else if(comboState == 16){ //^ 16: intake reversing, wall mech loading
            intake.move_voltage(-intakeVoltage);
            WMTarget = WMLoadingTarget;
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 17){ //^ 17: secret state where just the bottom is intaking. unable to use controller to get here though it's only for autons i guess
            intakeTop.brake();
            intakeBottom.move_voltage(intakeVoltage);
        }
        else if(comboState == 18){ //^ 18: second secret state where the bottom's outtaking while the top's intaking. controller unable to reach this state
            intakeTop.move_voltage(intakeVoltage);
            intakeBottom.move_voltage(-intakeVoltage);
        }
        else if(comboState == 19){ //^ 19: third secret state where its 17 but the wall mech is up
            WMTarget = WMScoringTarget;
            intakeTop.brake();
            intakeBottom.move_voltage(-intakeVoltage);
        }
        else if(comboState == 20){ //^ 20: dejam
            prevComboState = 20;
            dejam(dejamIncomingState);
        }
        else if(comboState == 21){//^ 21: intake running, wall mech out forward
            WMTarget = WMForwardTarget;
            if(justLifted){delay(100); justLifted = 0;}
            intake.move_voltage(intakeVoltage);
            if(colorSorting){opticalSensor.set_led_pwm(100.0);}
            else{opticalSensor.set_led_pwm(0.0);}
        }
        else if(comboState == 22){ //^ 22: intake stopped, wall mech out forward
            WMTarget = WMForwardTarget;
            intake.brake();
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 23){ //^ 23: hang state (intake stopped, wall mech barely high enough to get the hang bars up)
            WMTarget = WMHangTarget;
            intake.brake();
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 24){ //^ 24: color sort out of forward position continuous
            WMTarget = WMForwardTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(24);}
            else{comboState = 21;}
            prevComboState = 21;
        }
        else if(comboState == 25){ //^ 25: reverse while wall mech forward
            WMTarget = WMForwardTarget;
            intake.move_voltage(-intakeVoltage);
            opticalSensor.set_led_pwm(0.0);
        }
        else if(comboState == 26){ //^ 26: intake running until finding correct ring, wall mech forward
            WMTarget = WMForwardTarget;
            intake.move_voltage(intakeVoltage);
        }
        else if(comboState == 27){ //^ 27: color sort out of forward position until finding correct ring
            WMTarget = WMForwardTarget;
            intake.move_voltage(intakeVoltage);
            if(colorSorting){colorSort(27);}
            else{comboState = 26;}
            prevComboState = 26;
        }

        prevBackClawBool = backClawBool;
        delay(10);
    }
}
float WMDistance;
void runWallMech(){ //also holds printing so we only print in one task
    while(1){
        if(WMTarget != prevWMTarget){
            WMError = 0.0;
            WMIntegral = 0.0;
            WMDerivative = 0.0;
        }
        WMPosition = WMPotentiometer.get_value();
        WMError = WMTarget - WMPosition;
        WMIntegral += WMError;
        if(WMTarget > 160.0 && getDir(WMIntegral) != getDir(WMError)){WMIntegral = 0.0;}
        //if(fabs(WMIntegral * WMKi) >= WMIntegralMax){WMIntegral = getDir(WMIntegral) * WMIntegralMax / WMKi;}
        //if(fabs(WMError) > WMErrorMax){WMIntegral = 0.0;}
        WMDerivative = WMPreviousPos - WMPosition;
        if(WMTarget <= 45.0){WMIntegral = 0.0; WMDerivative = 0.0;}
        WMPower = WMKp * WMError + WMKi * WMIntegral + WMKd * WMDerivative + (WallMechAntiGravTuner * 500.0 * pow(eConst, -pow(WMPosition/1000.0,2.0)) - 125.0);
        if((WMTarget < 400.0) && (WMPosition < 400.0)) { WMPower = 0.0; }
        WMPreviousPos = WMPosition;
        wallMech.move_voltage(WMPower);
        prevWMTarget = WMTarget;



        if(controller.get_digital_new_press(backClawTog)){backClaw.set_value(!backClaw.get_value());}
        
        //if(backClawLim1.get_value() && backClawLim2.get_value() && backClawBool == 0){backClaw.set_value(1);}
        if(backClawDis1.get() <= backClawDisLimit){backClawDisTrigger1 = 1;}
        else{backClawDisTrigger1 = 0;}
        if(backClawDis2.get() <= backClawDisLimit){backClawDisTrigger2 = 1;}
        else{backClawDisTrigger2 = 0;}
        if(backClawDisTrigger1 && backClawDisTrigger2 && (autoClampBlocked == 0) && (controller.get_digital(backClawTog) == 0)){backClaw.set_value(1); autoClampBlocked = 1; backClawBool = 1;}
        else if((backClawDisTrigger1 == 0) || (backClawDisTrigger2 == 0)){autoClampBlocked = 0;}



        
        //debug combo system
        
        
        WMDistance = intakeDistanceSensor.get();
        /*
        lcd::clear();
        lcd::print(0, "%d : combo state", comboState);
        lcd::print(1, "%d : color sorting", colorSorting);
        if(teamColor == COLOR_RED){lcd::print(2, "RED : team color");}
        else{lcd::print(2, "BLUE : team color");}
        lcd::print(3, "%d : checkDejam", checkDejam);
        lcd::print(4, "%f : dejamTimer", dejamtimer);
        lcd::print(5, "%f : intake velocity", intakeTop.get_actual_velocity());
        */

        //lcd::print(2, "%f : distance of WM sensor", WMDistance);
        //lcd::print(3, "%f : position of WM", WMPosition);
        //lcd::print(4, "%f : target of WM", WMTarget);
        // lcd::print(3, "%f : redquotient", opticalSensor.get_raw().red / opticalSensor.get_brightness());
        // lcd::print(4, "%f : bluequotient", opticalSensor.get_raw().blue / opticalSensor.get_brightness());
        //lcd::print(5, "%f : error of WM", WMError);
        //lcd::print(6, "%f : power of WM", WMPower);
        //lcd::print(7, "%d : back claw", backClawBool);
        //lcd::print(7, "%f : WMi", WMKi * WMIntegral);
        //lcd::print(6, "%d : disbool 1", backClawDisTrigger1);
        //lcd::print(7, "%d : disbool 2", backClawDisTrigger2);        
        //lcd::set_text(7, std::to_string(opticalSensor.get_proximity()));        
        
        //std::cout << WMPotentiometer.get_value() << "\n";
        

        //odom output
        /*
        lcd::clear();
        //lcd::print(0, "%f : xPos", xPos);
        //lcd::print(1, "%f : yPos", yPos);
        lcd::set_text(0, std::to_string());
        lcd::set_text(1, std::to_string(yPos));
        lcd::print(2, "%f : tPos", tPos);
        lcd::print(3, "%f : r1pos", xTracking.get_position());
        lcd::print(4, "%f : r2pos", yTracking.get_position());
        lcd::set_text(5, std::to_string(xTracking.get_position()));
        lcd::set_text(6, std::to_string(yTracking.get_position()));
        */

        /*
        lcd::print(0, "%f : xPos", xPos);
        lcd::print(1, "%f : yPos", yPos);
        lcd::print(2, "%f : tPos", tPos * 180.0 / pi);

        lcd::print(5, "%f : WM pos", WMPosition);
        lcd::print(6, "%f : WM target", WMTarget);
        lcd::print(7, "%f : todeg(norm(getang()))", 180.0 / pi * normAngle(getAngle()));
        */
        //lcd::print(4, "%d : comboState", comboState);

        
        if(teamColor == COLOR_RED){printTeamBool = 1;}
        else{printTeamBool = 0;}

        if(controller.get_digital(DIGITAL_R1) && controller.get_digital(DIGITAL_R2) && controller.get_digital(DIGITAL_L1) && controller.get_digital(DIGITAL_L2)){std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<< "\n";}
        else{std::cout << "redQuotient: " << opticalSensor.get_raw().red / opticalSensor.get_brightness() << " - " "opticalProximity: " << opticalSensor.get_proximity() << " - " << "comboState: " << comboState << " - " << "backClawBool: " << backClawBool << " - " << "specialIntake: " << specialIntake << " - " << "teamBool: " << printTeamBool << " - " << "dejamTimer: " << dejamtimer << " - " << "checkDejam: " << checkDejam << " - " << "dejamIncomingState: " << dejamIncomingState << " - " << "forcedTransit: " << forcedTransit << " - " << "forcedState: " << forcedState << " - " << "\n" << "\n";}
        

        delay(10);
    }
}

void transit(int forcedStateInput){ //force exit from color sort in auton
    comboState = forcedStateInput;
    forcedTransit = 1;
    forcedState = forcedStateInput;
    checkDejam = 0;
}

void dejam(int incomingState){
    if (intakeTop.get_actual_velocity() > dejamBarrier){
        dejamtimer = 0;
        transit(incomingState);
        if(incomingState < 10 || incomingState > 15){forcedTransit = 0;}
    }
    else{
        dejamtimer++;
        //transit(incomingState);
        //checkDejam = 1;
    }

    if (dejamtimer > dejamtimerlimit){
        intake.move_voltage(-intakeVoltage);
        delay(167);
        transit(incomingState);
        if(incomingState < 10 || incomingState > 15){forcedTransit = 0;}
    }
}