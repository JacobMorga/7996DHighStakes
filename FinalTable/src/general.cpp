#include "main.h"
using namespace std;

// Devices
Controller controller (CONTROLLER_MASTER);

Motor drive1 (15, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive2 (16, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive3 (13, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1, drive2, drive3});

Motor drive4 (14, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive5 (12, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive6 (11, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4, drive5, drive6});

Motor_Group drivetrain ({drive1, drive2, drive3, drive4, drive5, drive6});

Motor intakeTop (6, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor intakeBottom (20, MOTOR_GEAR_200, true, MOTOR_ENCODER_DEGREES);
Motor_Group intake ({intakeTop, intakeBottom});

Motor wallMech (9, MOTOR_GEAR_200, true, MOTOR_ENCODER_DEGREES);
ADIPotentiometer wallMechPotentiometer ('B', pros::E_ADI_POT_EDR); // second argument declares potentiometer type

Rotation xTracking (7);
Rotation yTracking (19);

IMU inertial1 (1);
IMU inertial2 (2);
IMU inertial3 (4);

ADIPort backClaw ('H', ADI_DIGITAL_OUT);
ADIPort rightClearer ('B', ADI_DIGITAL_OUT);
ADIPort leftClearer ('F', ADI_DIGITAL_OUT);
ADIPort intakePiston ('G', ADI_DIGITAL_OUT);

Optical opticalSensor(10);
Distance distanceSensor(5);
Distance WMDistanceSensor(8);

struct coord {

    float x;
    float y;

    // Constructors
    coord();
    coord(float xIN, float yIN){
        x = xIN;
        y = yIN;
    }
};

void printAtPoint(text_format_e_t txtFmt, int x, int y, const char* text){

    pros::c::screen_print_at(txtFmt,x,y,text);
}

//Constants
const float pi = 3.1415927410125732421875; // Cir/Dia
const bool pressed = 1;
const bool unpressed = 0;
const bool on = 1;
const bool off = 0;
const float reverse = -1.0;

float returnSmaller (float x, float y){
    if (x < y){ return x; } // x is smaller
    else { return y; } // y is smaller or equal
}

float returnBigger (float x, float y){
    if (x > y){ return x; } // x is bigger
    else { return y; } // y is bigger or equal
}

float getDir(float input){

    if (input >= 0.0){ return 1.0; }
    else if (input < 0.0){ return -1.0; }
    else { return 0.0; }
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
    
    dif12 = fabs(inertial1value - inertial2value); // Find difference between each inertial value
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

float to_float (int n){ return (static_cast<float> (n)); }

float distance(float x1, float y1, float x2, float y2){

    return sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));
}

float pythagThisJohn (float a, float b){

    return sqrt(pow(a, 2.0) + pow(b, 2.0));
}

// Arc Tan 2 Function

float a;
float arctan2(float x, float y){ //returns angle from positive x axis on (-pi, pi]
    a = 0.0;
    if (x > 0.0){a = atan(y / x);}
    else if (x < 0.0){
        if (y >= 0.0){a = atan(y / x) + pi;}
        else{a = atan(y / x) - pi;}
    }
    else{
        if (y > 0.0){a = pi / 2.0;}
        else if (y < 0.0){a = -pi / 2.0;}
        else{a = 0.0;}
    }
    return a;
}

float normAngle(float angle){
    while (angle > pi){angle -= 2.0*pi;}
    while (angle < -pi){angle += 2.0*pi;}
    return angle;
}

float normAngle2pi(float angle){
    while (angle < 0){angle += 2.0*pi;}
    while (angle > 2.0*pi){angle -= 2.0*pi;}
    return angle;
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