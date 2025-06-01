#include "main.h"


/** motionAlgs.cpp - Motion Algorithms 
 * 
 * This file contains motion algorithms to control the robot's movement on the field.
 * Functions include:
 * - facePoint: Aligns the robot to a specific point on the field. Useful for targeting game objects on matter where you are.
 * - toPoint: Moves the robot to a specific point on the field. Rotates and drives at the same time.
 * - toPointShortBy: Moves the robot to a point offset by a certain distance. Useful for lining up at a certain radius from an object. (High Stakes wall stakes)
 * - faceAway: Aligns the robot to face away from a specific point. (Reverse of toPoint)
 * - faceHeading: Aligns the robot to a specific heading.
 * 
 * All these functions use the basic theory of PIDs combined with the information from  odometry to create complex motion algorithms that allow more efficient, faster, and more complex autonomous routines
 */

// Linear PID tuning values (front to back error) (Previous & useful values stored in comments) (# = Variable expanded name)
//? Note to future teams - these values used should not be used as "starting values" for your own PIDs, as these are just scaling vales the numbers that work for us and our robot will NOT work for you.
float linKP = 550.0;//500.0; //# Linear Constant of Proportion
float linKI = 3.0;//1.0; //# Linear Constant of Integral
float linKD = 5000.0;//5000.0; //# Linear Constant of Derivative

// Rotation PID tuning values (Yaw) (Previous & useful values stored in comments)
float rotKP = 25000.0; //15000.0; //# Rotational Constant of Propotion
float rotKI = 1000.0; //750.0; //# Rotational Constant of Integral
float rotKD = 175000.0; //100000.0; //106250.0; //# Rotational Constant of Derivative

// Exit condition tuning values
const float tErrorMin = -1.0; //# Theta Error Minimum 
const float tErrorMax = 5.0 / 180.0 * pi; //# Theta Error Maximum
const float tIntMax = 2000.0; //# Theta Integration Maximum

// Variable initialization
float tError = 0.0; //# Theta Error
float tInt = 0.0; //# Theta Intergal
float tDer = 0.0; //# Theta Derivative
float tPrevError = 0.0; //# Theta Previous Error
float tPow = 0.0; //# Theta Power
float tTarget = 0.0; //this is a variable while tTar is an input //# Theta Target
int exitLoops = 0; //universal //# Exit Loops
bool pseudoVelSwitch = 0; //# Pseudo Velocity Switch
float pseudoVelLimit = 1.0; //# Pseudo Velocity Switch
float pseudoRotVelLimit = 1.0; //# Pseudo Rotational Velocity Limit
float maxAcceptableRotError = 1.5; //# Maximum Acceptable Rotational Error
float smoothFPError = 5.0; //# Smooth Face Point Error
bool checkPseudoVel = 1; //# Check Pseudo Velocity

/**
 * facePoint
 *  Takes a (x,y) coordinate for the point to face, a boolean to activate smooth, and a maximum cap on the speed during the maneuver
 *      xTar - (float, inches) X Position for the target point to face //# X Target
 *      yTar - (float, inches) Y Position for the target point to face //# Y Target
 *      fpSmooth - (boolean) 1 to activate "smooth" which exits the function early to provide faster and more fluid routines at the cost of exact precision //# Face Point Smooth
 *      speedLimit - (float, milliVolts) Caps the maximum speed of the drive during the manuever
 * 
 * !Pseudo Velocity:
 *      Pseduo velocity is a measurement created to check the robots relative speed. The variable was created as a means to check if the robot was stuck. We call it pseudo velocity because it is how far the robot moved in the last loop of odometry, similar to velocity but not quite.
 */
void facePoint(float xTar, float yTar, bool fpSmooth, float speedLimit){
    // Reset variables
    pseudoVelSwitch = 0;
    exitLoops = 0;
    while (exitLoops < 25){ // Run until the robot is within target for so long
        tPos = (pi / 2.0) - tPos; // Set "0 Degrees" -> "pi/2 Radians" for easier math //! Not exactly sure how this works 
        tTarget = arctan2(xTar - xPos, yTar - yPos); // Calculate theta target (Rotation from X-Axis, updates every loop)
        tError = normAngle(tTarget - tPos); // Find the error between robots current heading and heading to target (Normalize to -pi to pi)
        tInt += tError; // Update integral value
        if (fabs(tError) >= tErrorMax || fabs(tInt) >= tIntMax){tInt = 0.0;} // Set limits on integral
        tDer = tError - tPrevError; // Calculate theta derivative 
        tPrevError = tError; // Update error for next loop
        tPow = rotKP * tError + rotKI * tInt + rotKD * tDer; // Calculate power by multiply by scaling constants
        if(fabs(tPow) > speedLimit){tPow = getDir(tPow) * speedLimit;} // Set power to power limit if above
        rightDrive.move_voltage(tPow); // Move drive motors
        leftDrive.move_voltage(-tPow);

        if(pseudoVelSwitch == 0 && fabs(pseudoRotVel) > pseudoRotVelLimit){pseudoVelSwitch = 1;} // Prevents pseudo velocity from triggering right at start of the movement when the robot is not moving yet

        if (fabs(tError / pi * 180.0) <= maxAcceptableRotError || (pseudoVelSwitch == 1 && fabs(pseudoRotVel) < pseudoRotVelLimit)){exitLoops += 1;} // If the robot is within a certain amount from the target or the robot has stopped, start exiting the loop
        else if(fpSmooth == 1 && fabs(tError / pi * 180.0) < smoothFPError){exitLoops += 25;} // Second trigger point with a much larger zone for linking movements together, exits immediatly once robot hits the target zone
        else{exitLoops = 0;} // If doesn't meet any condition reset

        delay(10);
    }
    drivetrain.brake(); // Stop drive at end of movement (motors will stay at last commanded velocity if not set to 0)
}

const float lErrorMin = 0.25; 
const float lIntMax = 1000.0;

float lError = 0.0;
float lInt = 0.0;
float lDer = 0.0;
float lPrevError = 0.0;
float lPow = 0.0;

int linearLoops = 0;
float initPos = 0.0;
float currentPos = 0.0;

int toPointLoops = 0;
float maxDist = 0.0;
float dist = 0.0;
float rightPow = 0.0;
float leftPow = 0.0;
const float tWeightK = 0.257; //*tune this
float tWeightA = 0.0;
float tWeightB = 0.0;
float tWeight = 0.0;
float deadZoneRadius = 12.0;
float ttotara = 0.0;
float ttotarb = 0.0;
float ttotaraerr = 0.0;
float ttotarberr = 0.0;
float tpl2dir = 0.0;
float ttotar = 0.0;
float distLimit = 5.0;

float TProtKP = 30000.0; //best so far 15000.0;
float TProtKI = 0.0; //best so far 0.0;
float TProtKD = 500000.0; //best so far 500000.0;

float offsetDist = 0.0;
float angleToTarget = 0.0;
bool updateTargets = 0;
float xTarShortInput = 0.0;
float yTarShortInput = 0.0;

bool TPSB1 = 1;

void toPoint(float xTar, float yTar, float reversed, bool smooth, float powerLimit){

    // Reset and init variables 
    xTar *= fieldRatio; // Sets the target in field tiles lengths to inches (Field tiles are not exactly 48 inches so when we say "move forward 48 in" we want it to move one field tiles and thus it need to move slightly less than 48 in)
    yTar *= fieldRatio;
    toPointLoops = 0;
    tInt = 0.0;
    tDer = 0.0;
    tPrevError = 0.0;
    lInt = 0.0;
    lDer = 0.0;
    lPrevError = 0.0;
    pseudoVelSwitch = 0;
    pseudoVelocity = 0.0;
    maxDist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0));
    while (toPointLoops < 50){

        if(updateTargets){ // Offsets targets if short-by is used
            xTar = xTarShortInput - offsetDist * cos(angleToTarget);
            yTar = yTarShortInput - offsetDist * sin(angleToTarget);
        }

        tPos = (pi / 2.0) - tPos + pi * reversed;// Set "0 Degrees" -> "pi/2 Radians" for easier math (Sets heading opposite for reverse driving manuvers) //! Not exactly sure how this works 
        tTarget = arctan2(xTar - xPos, yTar - yPos); // Calculate the radians to the target point
        tError = normAngle(tTarget - tPos); // Find error and normalise to -pi to pi
        tInt += tError; // Update integral
        tDer = tError - tPrevError; // Update derivative
        tPrevError = tError; // Update previous error
        tPow = TProtKP * tError + TProtKI * tInt + TProtKD * tDer; // Calculate robots desired tuning power

        dist = sqrt(pow(xTar - xPos, 2.0) + pow(yTar - yPos, 2.0)); // Find distance to target
        lError = (1.0 - 2.0 * reversed) * dist * cos(tError); // Find cos of the error (prioritize turning before going straight)
        lInt += lError; // Update integral
        if (fabs(lError) <= lErrorMin || fabs(lInt) >= lIntMax){lInt = 0.0;} // Limit integral power
        lDer = lError - lPrevError; // Update derivative 
        lPrevError = lError; // Update previous error
        lPow = linKP * lError + linKI * lInt + linKD * lDer; // Calculate power desired for linear power

        // Scale turning power based no distance to target (As you get close your delta theta can get really high as you are close so you need to limit it)
        tWeightA = (1.0 + sqrt(1.0 + 4.0 / tWeightK)) / 2.0;
        tWeightB = 1.0 / (tWeightK * (tWeightA - 1.0));
        tWeight = 1.0 / (tWeightK * (1.0 - (dist / maxDist) - tWeightA)) + tWeightB;

        rightPow = lPow + tWeight * tPow; // Calculate right and left motor power
        leftPow = lPow - tWeight * tPow;

        // Scale motor power if one is above motor input limit (If the robot wants to turn and go straight at full power the code will output 24,000mV on one side and 13,000mV on the other, the code is trying to turn but the robot will just go straight as 12,000mV is the max.
        if (fabs(rightPow) >= powerLimit || fabs(leftPow) >= powerLimit){
            if (fabs(rightPow) > fabs(leftPow)){
                rightPow = getDir(lPow + tWeight * tPow) * powerLimit;
                leftPow = getDir(lPow - tWeight * tPow) * fabs(powerLimit * (lPow - tWeight * tPow) / (lPow + tWeight * tPow));
            }
            else{
                rightPow = getDir(lPow + tWeight * tPow) * fabs(powerLimit * (lPow + tWeight * tPow) / (lPow - tWeight * tPow));
                leftPow = getDir(lPow - tWeight * tPow) * powerLimit;
            }
        }

        rightDrive.move_voltage(rightPow); // Spin motors
        leftDrive.move_voltage(leftPow);


        if(pseudoVelSwitch == 0 && pseudoVelocity > pseudoVelLimit){pseudoVelSwitch = 1;} // Prevents pseudo velocity from triggering right at start of the movement when the robot is not moving yet

        if (dist < distLimit || (pseudoVelSwitch == 1 && pseudoVelocity < pseudoVelLimit && checkPseudoVel == 1)){ // If the robot is within a certain amount from the target or the robot has stopped, start exiting the loop
            if (smooth == 0){toPointLoops += 1;}
            else{toPointLoops += 10;} // Exits earlier if smooth
        }
        else{toPointLoops = 0;} // If doesn't meet any condition reset

        delay(10);
    }
    drivetrain.brake();
    updateTargets = 0;
}

void toPointShortBy(float xTar, float yTar, float reversed, bool smooth, float offsetDist, float powerLimit){ // Calculates toPoint path that is short by so many inches, example if you wanted to grab a goal in the corner you would put in the coordinates of the corner and then short by so many inches for where the robot should stop.
    float angleToTarget = arctan2(xTar - xPos, yTar - yPos);
    xTarShortInput = xTar;
    yTarShortInput = yTar;
    if(TPSB1){updateTargets = 0;}
    else{updateTargets = 1;}
    toPoint(xTar - offsetDist * cos(angleToTarget), yTar - offsetDist * sin(angleToTarget), reversed, smooth, powerLimit);
}

void faceAway(float xTar, float yTar, bool fpSmooth, float speedLimit){ // Uses toPoint to face away from a point instead of at it
    facePoint(2.0 * xPos - xTar, 2.0 * yPos - yTar, fpSmooth, speedLimit);
}

void faceHeading(float tTar, bool fpSmooth, float speedLimit){ // Faces a certain global heading instead of point
    tTar *= pi / 180.0; // Degrees to radians 
    facePoint(xPos + 96.0 * cos(tTar), yPos + 96.0 * sin(tTar), fpSmooth, speedLimit);
}