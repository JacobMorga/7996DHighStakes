#include "main.h"

float rightDrivePow = 0.0;
float leftDrivePow = 0.0;
float joystickRYVal = 0.0;
float joystickLYVal = 0.0;
float joystickRXVal = 0.0;
float joystickLXVal = 0.0;

bool buttonR2Prev = 0;

int intakeState = 0; // 0- stopped  1- intake  2- outtake
int joysticks = 1;

void runDriveCont (){

    while (true){

        joystickRYVal = powf(controller.get_analog(ANALOG_RIGHT_Y) / 127.0 * 100.0, 3.0) / 10000.0 * 6.0; // Scales 127 to 100 then cubes and
        joystickLYVal = powf(controller.get_analog(ANALOG_LEFT_Y) / 127.0 * 100.0, 3.0) / 10000.0 * 6.0; // Scales 127 to 100 then cubes and
        joystickRXVal = powf(controller.get_analog(ANALOG_RIGHT_X) / 127.0 * 100.0, 3.0) / 10000.0 * 6.0; // Scales 127 to 100 then cubes and
        joystickLXVal = powf(controller.get_analog(ANALOG_LEFT_X) / 127.0 * 100.0, 3.0) / 10000.0 * 6.0; // Scales 127 to 100 then cubes and

        if (joysticks == 1){
            rightDrivePow = joystickRYVal - joystickLXVal;
            leftDrivePow = joystickRYVal + joystickLXVal;
        }
        if (joysticks == 2){
            rightDrivePow = joystickRYVal - (joystickRXVal + joystickLXVal);
            leftDrivePow = joystickLYVal + (joystickRXVal + joystickLXVal);  
        }
        
        rightDrive.move_velocity(rightDrivePow); // Sets motors to move
        leftDrive.move_velocity(leftDrivePow);

        if (fabs(rightDrivePow) < 5 && fabs(leftDrivePow) < 5){ // Brakes drive when joytsick hits zero
            rightDrive.brake();
            leftDrive.brake();
        }

        if (intakeState == 2){intake.move_velocity(-600);}
        else if (intakeState == 1){intake.move_velocity(600);}
        else{intake.brake();}

        if (controller.get_digital_new_press(DIGITAL_R2) == pressed){
            intakeState = abs(intakeState - 1);
        }
        if (controller.get_digital(DIGITAL_R1) == pressed){
            intakeState = 2;
        }
        if (controller.get_digital(DIGITAL_R1) == unpressed && buttonR2Prev == pressed){
            intakeState = 0;
        } 

        buttonR2Prev = controller.get_digital(DIGITAL_R1);

        if (controller.get_digital_new_press(DIGITAL_L2) == pressed){
            backClaw.set_value(!backClaw.get_value());
        }
        if (controller.get_digital_new_press(DIGITAL_L1) == pressed){
            ploinker.set_value(!ploinker.get_value());
        }
        if (controller.get_digital_new_press(DIGITAL_LEFT) == pressed){
            joysticks = 3 - joysticks;
        }

        lcd::set_text(7, std::to_string(drive1.get_actual_velocity()));
        lcd::set_text(6, std::to_string(drive4.get_actual_velocity()));
        

        delay(20);
    }
}