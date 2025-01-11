#include "main.h"
using namespace std;

// Devices
Controller controller (CONTROLLER_MASTER);

Motor drive1 (18, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive2 (14, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive3 (1, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1, drive2, drive3});

Motor drive4 (9, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive5 (6, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive6 (7, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4, drive5, drive6});

Motor_Group drivetrain ({drive1, drive2, drive3, drive4, drive5, drive6});

Motor intakeTop (13, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor intakeBottom (17, MOTOR_GEAR_200, true, MOTOR_ENCODER_DEGREES);
Motor_Group intake ({intakeTop, intakeBottom});

Rotation xTracking (20);
Rotation yTracking (21);

IMU inertial1 (5);
IMU inertial2 (12);
IMU inertial3 (16);

ADIPort backClaw ('A', ADI_DIGITAL_OUT);
ADIPort ploinker ('H', ADI_DIGITAL_OUT);

Optical opticalSensor(10);
Distance distanceSensor(8);

//Constants
const float pi = 3.1415927410125732421875; // Cir/Dia
const bool pressed = 1;
const bool unpressed = 0;
const bool on = 1;
const bool off = 0;
const float reverse = 1.0;

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
float inertial1value, inertial2value, inertial3value;
float dif12, dif23, dif13;
float leastDif, angle;
int difSelect;
float getAngle(void){
    inertial1value = inertial1.get_rotation();
    inertial2value = inertial2.get_rotation();
    inertial3value = inertial3.get_rotation();
    
    dif12 = fabs(inertial1value - inertial2value); // Find differnce between each inertial value
    dif23 = fabs(inertial2value - inertial3value);
    dif13 = fabs(inertial1value - inertial3value);

    leastDif = dif12; difSelect = 1; // Select smallest difference
    if (dif23 <= leastDif){leastDif = dif23; difSelect = 2;}
    if (dif13 <= leastDif){leastDif = dif13; difSelect = 3;}
    
    if (difSelect == 1){angle = 0.5 * (inertial1value + inertial2value);} // Evaluate average of the two closest sensors
    else if (difSelect == 2){angle = 0.5 * (inertial2value + inertial3value);}
    else{angle = 0.5 * (inertial1value + inertial3value);}

    //angle = (pi / 2.0) - (angle / 180.0 * pi); // Convert to radians and have zero heading pi/2 rad
    angle = angle / 180.0 * pi;

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

/*
float arctan2(float x, float y){
    float a = 0.0;
    if (x > 0.0){
        a = (atan(y / x));
    }
    else if (x < 0.0){
        a = (atan(y / x) + pi);
    }
    else if (x == 0.0){
        if (y > 0){
            a = (pi / 2.0);
        }
        if (y < 0){
            a = (1.5 * pi);
        }
    }
    return a;
}
*/

float normAngle(float angle){
    while (angle > pi){angle -= 2.0 * pi;}
    while (angle < -pi){angle += 2.0 * pi;}
    //return angle;
}

float normAngle2(float angle){
    angle = (pi / 2.0) - (angle / 180.0 * pi);
    while (angle > (2.0 * pi)) { angle -= 2.0 * pi; } 
    while (angle < 0.0) { angle += 2.0 * pi; }
    return angle;
}

float normAngle3(float angle){
    while (angle > pi){angle -= 2.0 * pi;}
    while (angle < -pi){angle += 2.0 * pi;}
    return angle;
}

template <typename anyVar>
void printToBrain(int lineNum, anyVar input){
    
    lcd::set_text(lineNum, std::to_string(input));
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

int timer; // ms
void motorTesting (){

    timer = 0;

    drive1.move_voltage(13000); // PORT 13
    drive1.set_brake_mode(MOTOR_BRAKE_COAST);
    drive1.set_encoder_units(MOTOR_ENCODER_DEGREES);

    while(timer < 30000){ // ms

        cout << timer << ",";
        cout << drive1.get_actual_velocity() << ",";
        cout << drive1.get_current_draw() << ",";
        cout << drive1.get_efficiency() << ",";
        cout << drive1.get_position() << ",";
        cout << drive1.get_power() << ",";
        cout << drive1.get_temperature() << ",";
        cout << drive1.get_torque() << ",";
        cout << drive1.get_voltage() << "," << "\n";

        delay(10);
        timer += 10;
    }

    drive1.brake(); // PORT 13
}