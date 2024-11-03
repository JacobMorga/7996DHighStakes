#include "main.h"


float joystickYVal = 0.0;
float joystickXVal = 0.0;
float rightDrivePow = 0.0;
float leftDrivePow = 0.0;

bool buttonR2Prev = 0;

int intakeState = 0; // 0- stoped  1- intake  2- outtake

void runDriveCont (){

    while (true){

        joystickYVal = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 10000.0 * 6.0; // Scales 127 to 100 then cubes and
        joystickXVal = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 10000.0 * 6.0; // scales back to 100 then multipes to 600 scale

        rightDrivePow = joystickYVal - joystickXVal;
        leftDrivePow = joystickYVal + joystickXVal;
        
        rightDrive.move_velocity(rightDrivePow); // Sets motors to move
        leftDrive.move_velocity(leftDrivePow);



        if (fabs(rightDrivePow) < 5 && fabs(leftDrivePow) < 5){ // Brakes drive when joytsick hits zero
            rightDrive.brake();
            leftDrive.brake();
        }



        if (intakeState == 2){intake.move_velocity(-600);}
        else if (intakeState == 1){intake.move_velocity(600);}
        else{intake.brake();}

        if (controller.get_digital_new_press(DIGITAL_R1) == pressed){
            intakeState = abs(intakeState - 1);
        }
        if (controller.get_digital(DIGITAL_R2) == pressed){
            intakeState = 2;
        }
        if (controller.get_digital(DIGITAL_R2) == unpressed && buttonR2Prev == pressed){
            intakeState = 0;
        }



        buttonR2Prev = controller.get_digital(DIGITAL_R2);

        if (controller.get_digital_new_press(DIGITAL_L1) == pressed){
            backClaw.set_value(!backClaw.get_value());
        }
        if (controller.get_digital_new_press(DIGITAL_L2) == pressed){
            ploinker.set_value(!ploinker.get_value());
        }

        delay(20);
        // Test
        //Bibg balls
        //Big g monry
        // bigger G money
    }
}