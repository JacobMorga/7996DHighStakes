#include "main.h"
using namespace pros;

float rightDrivePow, leftDrivePow;
float JRYValue, JRXValue, JLYValue, JLXValue;

int intakeState = 0;
bool buttonR2Prev = 0;

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

        if (fabs(rightDrivePow) < 120 && fabs(leftDrivePow) < 120){ // Brakes drive when joytsick hits zero (1% power)
            rightDrive.brake();
            leftDrive.brake();
        }

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

        if (controller.get_digital_new_press(DIGITAL_L2) == pressed){ // Toggles backclaw
            backClaw.set_value(!backClaw.get_value());
        }
        if (controller.get_digital_new_press(DIGITAL_L1) == pressed){ // Toggles ploinker
            ploinker.set_value(!ploinker.get_value());
        }
    }
}