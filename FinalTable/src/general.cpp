#include "main.h"
using namespace std;

// Devices
Controller controller (CONTROLLER_MASTER);

Motor drive1 (18, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive2 (16, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor drive3 (20, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor_Group rightDrive ({drive1, drive2, drive3});

Motor drive4 (13, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive5 (12, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor drive6 (11, MOTOR_GEAR_600, false, MOTOR_ENCODER_DEGREES);
Motor_Group leftDrive ({drive4, drive5, drive6});

Motor_Group drivetrain ({drive1, drive2, drive3, drive4, drive5, drive6});

Motor intakeTop (17, MOTOR_GEAR_600, true, MOTOR_ENCODER_DEGREES);
Motor intakeBottom (14, MOTOR_GEAR_200, true, MOTOR_ENCODER_DEGREES);
Motor_Group intake ({intakeTop, intakeBottom});

Motor wallMech (1, MOTOR_GEAR_200, true, MOTOR_ENCODER_DEGREES);
ADIPort WMPotentiometer ('H', E_ADI_ANALOG_IN); // second argument declares potentiometer type //? ok but i just switched the potentiometer to v5 and didnt change this and it still works so idek but whatever //! was E_ADI_POT_EDR, now E_ADI_POT_V2

Rotation xTracking (3);
Rotation yTracking (9);
//Rotation intakeRotation (4);

IMU inertial1 (2);
IMU inertial2 (19);
IMU inertial3 (21);

//ADIDigitalOut backClaw ('G', 0);
//ADIDigitalOut rightClearer ('A');
//ADIDigitalOut leftClearer ('B');
//ADIDigitalOut intakePiston ('F');

ADIPort backClaw ('B', ADI_DIGITAL_OUT);
ADIPort rightClearer ('D', ADI_DIGITAL_OUT);
ADIPort leftClearer ('F', ADI_DIGITAL_OUT);
ADIPort intakePiston ('G', ADI_DIGITAL_OUT);

Optical opticalSensor(10);
Distance intakeDistanceSensor(4);
Distance WMDistanceSensor(7);

//ADILed led4 ('G', 33);

//ADIDigitalIn backClawLim1 ('F');
//ADIDigitalIn backClawLim2 ('G');

Distance backClawDis1 (6);
Distance backClawDis2 (15);

pros::ADILED stripRight('a',55);
pros::ADILED stripLeft('c',55);
std::vector<pros::ADILED> allStrips = {stripRight,stripLeft};


void printAtPoint(text_format_e_t txtFmt, int x, int y, const char* text){

    pros::c::screen_print_at(txtFmt,x,y,text);
}

//Constants
const float pi = 3.1415927410125732421875; // Cir/Dia
const float eConst = 2.718281828459045235360287471352;
const bool pressed = 1;
const bool unpressed = 0;
const bool on = 1;
const bool off = 0;
const float reverse = -1.0;
const float fieldRatio = 0.985026041667;

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

float deEed(float eNum){
    if(fabs(eNum) <= 0.0001){return 0;}
    else{return eNum;}
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
    

    //angle = inertial3.get_rotation();

    //angle = (pi / 2.0) - (angle / 180.0 * pi); // Convert to radians and have zero heading pi/2 rad
    angle = angle / 180.0 * pi;

    return angle;
}

float to_float (int n){ return (static_cast<float> (n)); }

float distance(float x1, float y1, float x2, float y2){
    return sqrtf(powf(x2-x1, 2.0) + powf(y2-y1, 2.0));
}

float pythag (float a, float b){
    return sqrtf(powf(a, 2.0) + powf(b, 2.0));
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

bool cR1,cR2,cL1,cL2,cA,cB,cX,cY,cUP,cDOWN,cRIGHT,cLEFT = false; // Current button presses 
vector<bool> cList = {cR1,cR2,cL1,cL2,cA,cB,cX,cY,cUP,cDOWN,cRIGHT,cLEFT}; // List of all current presses

bool pR1,pR2,pL1,pL2,pA,pB,pX,pY,pUP,pDOWN,pRIGHT,pLEFT = false; // Previous loop button presses
vector<bool> pList = {cR1,cR2,cL1,cL2,cA,cB,cX,cY,cUP,cDOWN,cRIGHT,cLEFT}; // List of all previous presses

//& 0: Not pressed
//& 1: Pressed
//& 2: Held
int sR1,sR2,sL1,sL2,sA,sB,sX,sY,sUP,sDOWN,sRIGHT,sLEFT = 0; // Button state
vector<int> sList = {cR1,cR2,cL1,cL2,cA,cB,cX,cY,cUP,cDOWN,cRIGHT,cLEFT}; // List of all current states

int tR1,tR2,tL1,tL2,tA,tB,tX,tY,tUP,tDOWN,tRIGHT,tLEFT = 0; // Button timers
vector<int> tList = {tR1,tR2,tL1,tL2,tA,tB,tX,tY,tUP,tDOWN,tRIGHT,tLEFT}; // List of all current timers

int buttonTimeLimit = 250; // time before click is considered a hold (milliseconds)
void USETHEBUTTONS (){

    while(1){

        // Get current loop values
        cR1    = controller.get_digital(DIGITAL_R1);
        cR2    = controller.get_digital(DIGITAL_R2);
        cL1    = controller.get_digital(DIGITAL_L1);
        cL2    = controller.get_digital(DIGITAL_L2);
        cA     = controller.get_digital(DIGITAL_A);
        cB     = controller.get_digital(DIGITAL_B);
        cX     = controller.get_digital(DIGITAL_X);
        cY     = controller.get_digital(DIGITAL_Y);
        cUP    = controller.get_digital(DIGITAL_UP);
        cDOWN  = controller.get_digital(DIGITAL_DOWN);
        cRIGHT = controller.get_digital(DIGITAL_RIGHT);
        cLEFT  = controller.get_digital(DIGITAL_LEFT);

        // Update list values
        cList = {cR1,cR2,cL1,cL2,cA,cB,cX,cY,cUP,cDOWN,cRIGHT,cLEFT};
        pList = {pR1,pR2,pL1,pL2,pA,pB,pX,pY,pUP,pDOWN,pRIGHT,pLEFT};

        //* Combo matrix
        //* c = current | p = previous
        //* c=0 && p=0 : set state to 0 and reset timer
        //* c=0 && p=1 : set state to 0 and reset timer
        //* c=1 && p=0 : set state to 1 and update timer
        //* c=1 && p=1 : if timer is above limit set state to 2, if not set to state 1

        // Compare values
        for (int i = 0; i < cList.size(); i++){

            if (cList[i] == 0){ // First two in matrix

                tList[i] = 0; // Reset timer
                sList[i] = 0; // Update state (not pressed)
            }
            else if (cList[i] == 1 && pList [i] == 0){ // Third in matrix
            
                tList[i] += 5; // Update timer
                sList[i] = 1; // New button press
            }
            else if (cList[i] == 1 && pList [i] == 1){ // Fourth in matrix

                tList[i] += 5; // Update timer
                if (tList[i] > buttonTimeLimit){ sList[i] = 2; } // Check to see it button is held
            }
            else{} // What the sigma
        }

        // Update previous values
        pR1    = cR1;
        pR2    = cR2;
        pL1    = cL1;
        pL2    = cL2;
        pA     = cA;
        pB     = cB;
        pX     = cX;
        pY     = cY;
        pUP    = cUP;
        pDOWN  = cDOWN;
        pRIGHT = cRIGHT;
        pLEFT  = cLEFT;

        delay(5);
    }
}

int getR1 (){
    if (sList[0] == 1){ sList[0] = 0; std::cout << "FUNCH"; return 1; }
    else{ return sList[0]; }
}
int getR2 (){
    if (sList[1] == 1){ sList[1] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[1]; }
}
int getL1 (){
    if (sList[2] == 1){ sList[2] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[2]; }
}
int getL2 (){
    if (sList[3] == 1){ sList[3] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[3]; }
}
int getA (){
    if (sList[4] == 1){ sList[4] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[4]; }
}
int getB (){
    if (sList[5] == 1){ sList[5] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[5]; }
}
int getX (){
    if (sList[6] == 1){ sList[6] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[6]; }
}
int getY (){
    if (sList[7] == 1){ sList[7] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[7]; }
}
int getUP (){
    if (sList[8] == 1){ sList[8] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[8]; }
}
int getDOWN (){
    if (sList[9] == 1){ sList[9] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[9]; }
}
int getRIGHT (){
    if (sList[10] == 1){ sList[10] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[10]; }
}
int getLEFT (){
    if (sList[11] == 1){ sList[11] = 0; std::cout << "FUNCH"; return 1; } 
    else{ return sList[11]; }
}


// ---------- LED STUFF -----------------

int rgbToHexInt(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

// 0 - shift it forward (back value moves to front)
// 1 - shift it backward (front value moves to back)
void shiftVector (std::vector<int>& inputV, bool direc){

	if (direc == false){
		int back = inputV.back();
		for(int i = inputV.size() - 1; i >= 1; i--){
			inputV[i] = inputV[i-1];
		}
		inputV[0] = back;
	}
	else{
		int front = inputV.front();
		for(int i = 0; i < inputV.size() - 1; i++){
			inputV[i] = inputV[i+1];
		}
		inputV.back() = front;

	}
}

// startcolor,endcolor,ledstrips
std::vector<int> colorGradientCalc (int sc, int ec, pros::ADILED& strip){

	// Calculate RGB values from hex codes
	float sRed = (sc >> 16) & 0xFF;
	float sGreen = (sc >> 8) & 0xFF;
	float sBlue = (sc) & 0xFF;

	float eRed = (ec >> 16) & 0xFF;
	float eGreen = (ec >> 8) & 0xFF;
	float eBlue = (ec) & 0xFF;

	std::vector<int> outputSteps; // Create and clear list
	outputSteps.clear();

	for (int i = 0; i < strip.length(); i++) { // Calculate RGB for each pixel
		float t = static_cast<float>(i) / static_cast<float>(strip.length()); // Pct along strip

		int r = static_cast<int>(sRed   + (eRed-sRed)     * t +0.5f); // Calc RGB values1
		int g = static_cast<int>(sGreen + (eGreen-sGreen) * t +0.5f);
		int b = static_cast<int>(sBlue  + (eBlue-sBlue)   * t +0.5f);

		std::cout << "(" << r << "," << g << ")" << ",";

		outputSteps.push_back(rgbToHexInt(r, g, b)); // Add to list
	}

	return outputSteps;
}


std::vector<std::vector<int>> colorTravelCalc (int startColor, int endColor, pros::ADILED& strip, bool direction){

	std::vector<std::vector<int>> setOfColors = {}; // Vectors for each step of colors
	std::vector<int> startingSet = colorGradientCalc(startColor,endColor,strip); // Basic set of colors
	setOfColors.push_back(startingSet); // push back initial set

	for(int i = 0; i < strip.length() - 1; i++){ // shift set a bunch of times and add to list
		shiftVector(startingSet,direction);
		setOfColors.push_back(startingSet);
	}

	return setOfColors;
}



void colorGradient(pros::ADILED& strip, std::vector<int> colorVector){

	for (int i = 0; i < strip.length(); i++) { // Get coresponding value

		strip[i] = colorVector[i];
		pros::delay(5);
	}
	strip.update();
}

void colorTravel(pros::ADILED& strip, std::vector<std::vector<int>> gradientVectors, int timeStep){

	for (std::vector<int> vector : gradientVectors){ // Get which color to use

		for (int i = 0; i < vector.size(); i++){ // Set lights

			strip[i] = vector[i];
		}
		strip.update();
		pros::delay(timeStep);

	}
}

void colorAlternate (std::vector<int> colors, pros::ADILED& strip, int timeStep){

	for(int i = 0; i < strip.length(); i++){

		strip[i] = colors[i % colors.size()];
	}
	strip.update();
}

void colorPulse(int startColor, int endColor, vector<pros::ADILED>& strips, int timeStep){
    for(int i = 0; i < strips[0].length(); i++){
        for (pros::ADILED& strip : strips){
            strip.set_all(colorGradientCalc(startColor,endColor,strip)[i]);
            strip.update();
        }
        delay(timeStep);
    }
}

int C_Blue = 0x6600ff;
int C_Red  = 0xff0000;



void DoTheLEDs (){
    while(1){
    //colorPulse(C_Red,0x770000,allStrips,250);
    stripRight.set_all(0xFF0000);
    stripRight.update();
    delay(100);
    }
}