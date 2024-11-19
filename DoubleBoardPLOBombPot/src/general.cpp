#include "main.h"
using namespace std;

// Devices
Controller controller (CONTROLLER_MASTER);

Motor drive1 (13, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive2 (14, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive3 (12, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1,drive2,drive3});

Motor drive4 (11, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive5 (15, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive6 (16, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4,drive5,drive6});

Motor_Group drivetrain ({drive1,drive2,drive3, drive4,drive5,drive6});

Motor intake (17, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);

Rotation xTracking (18);
Rotation yTracking (9);

IMU inertial1 (1);
IMU inertial2 (2);
IMU inertial3 (3);

ADIPort backClaw ('A', ADI_DIGITAL_OUT);
ADIPort ploinker ('B', ADI_DIGITAL_OUT);

//Constants
const float pi = 3.1415927410125732421875; // Cir/Dia
const bool pressed = 1;
const bool unpressed = 0;
const bool on = 1;
const bool off = 0;

template <typename anyVar>
float to_float (anyVar num){

    return static_cast<float>(num);
}

template <typename anyVar>
anyVar returnSmaller (anyVar x, anyVar y){

    if (x < y){ return x; } // x is smaller
    else { return y; } // y is smaller or equal
}

template <typename anyVar>
anyVar returnBigger (anyVar x, anyVar y){

    if (x > y){ return x; } // x is bigger
    else { return y; } // y is bigger or equal
}

template <typename anyVar>
anyVar getDir(anyVar input){

    if (input >= 0.0){
        return 1.0;
    }
    if (input < 0.0){
        return -1.0;
    }
}

// Get Angle Function 
float dif12, dif23, dif13;
float leastDif, angle;
int difSelect;
float getAngle(void){
    dif12 = fabs(inertial1.get_rotation() - inertial2.get_rotation()); // Find differnce between each inertial value
    dif23 = fabs(inertial2.get_rotation() - inertial3.get_rotation());
    dif13 = fabs(inertial1.get_rotation() - inertial3.get_rotation());

    leastDif = dif12; difSelect = 1; // Select smallest difference
    if (dif23 <= leastDif){leastDif = dif23; difSelect = 2;}
    if (dif13 <= leastDif){leastDif = dif13; difSelect = 3;}
    
    if (difSelect == 1){angle = 0.5 * (inertial1.get_rotation() + inertial2.get_rotation());} // Evaluate average of the two closest sensors
    else if (difSelect == 2){angle = 0.5 * (inertial2.get_rotation() + inertial3.get_rotation());}
    else{angle = 0.5 * (inertial1.get_rotation() + inertial3.get_rotation());}

    angle = (pi / 2.0) - (angle / 180.0 * pi); // Convert to radians and have zero heading pi/2 rad

    return angle;
}

float distance(float x1, float y1, float x2, float y2){

    return sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));
}

// Arc Tan 2 Function
float a;
float arctan2(float x, float y){
    a = 0.0;
    if (x > 0.0){a = atan(y / x);}
    else if (x < 0.0){
        if (y >= 0.0){a = atan(y / x) + pi;}
        else{a = atan(y / x) - pi;}
    }
    else{
        if (y > 0.0){a = pi / 2.0;}
        else if (y < 0.0){a = -3.0 * pi / 2.0;}
        else{a = 0.0;}
    }
    return a;
}

template <typename anyVar>
void printToBrain(int lineNum, anyVar input){
    
    lcd::set_text(lineNum, to_string(input));
}

template <typename anyVar>
void printToConsole(anyVar name, anyVar input){

    cout << name << ": " << input << "\n";
}

template <typename anyVar>
void printPointToConsole(anyVar name, anyVar x, anyVar y){

    cout << name << ": " << "{" << x << ", " << y << "}" << "\n";
}

void printAtPoint(text_format_e_t txtFmt, int x, int y, const char* text){

    pros::c::screen_print_at(txtFmt,x,y,text);
}
