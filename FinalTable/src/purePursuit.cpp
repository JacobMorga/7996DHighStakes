#include "main.h"
using namespace std;

int indexPP;
vector<coord> shiftedPath = {};
vector<coord> intersectionPoints = {};

float diffX,diffY,R,D;
float rightPowPP,leftPowPP = 0.0;

float int1Dist,int2Dist = 0.0;
bool intersection1Check, intersection2Check = true;
coord lastKnownIntersection (0.0,0.0);

int runPP = 0;
float tToTarget = 0.0;
float tErrorPP = 0.0;
float lErrorPP = 0.0;

coord robotPos (0.0,0.0);

coord followPoint;

//! NOTEBOOK NOTES
//! 1. IT CANT MOVE TO A POINT DIRECTLY AHEAD - VALUES OSCILATE BETWEEN 100M - -500M
//! 2. we calculated the distance to the endpoint wrong (intersection code rerturns distance from robot)
//! 3. it was using the behind point also, wasnt returning the end point of the function

coord findBestIntersection (vector<coord> path, float lookAheadDis, coord inputPoint){

    shiftedPath.clear();

    for (coord point : path){

        shiftedPath.push_back(coord(point.x - xPos, point.y - yPos)); // Shifts the point to put the robot position on the origin
    }

    indexPP = 0; // Lines distance along the path


    while(indexPP < shiftedPath.size() - 1){ // Runs loop for each pair of coordinates (each line)

        intersection1Check = true;  // Resets intersection checks
        intersection2Check = true;

        coord startPoint;
        startPoint.x = shiftedPath[indexPP].x; // Retrives x and y for each end point of line
        startPoint.y = shiftedPath[indexPP].y;

        coord endPoint;
        endPoint.x = shiftedPath[indexPP + 1].x;
        endPoint.y = shiftedPath[indexPP + 1].y;

        diffX = endPoint.x-startPoint.x;
        diffY = endPoint.y-startPoint.y;
        R = distance(startPoint.x,startPoint.y,endPoint.x,endPoint.y);
        D = startPoint.x * endPoint.y - endPoint.x * startPoint.y;

        coord int1; // first possible intersection
        int1.x = (D * diffY + getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0); // Calculates intersection points
        int1.y = (-D * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

        coord int2; // second possible intersection
        int2.x = (D * diffY - getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);
        int2.y = (-D * diffX - fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

        // Runs through checks to ensure intersection is on path
        if (isnan(int1.x)){ intersection1Check = false; intersection2Check = false;} // Check if there is an intersection

        if (int1.x == int2.x && int1.y == int2.y){ intersection2Check = false; } // Check if they are the same point

        if ( (int1.x > startPoint.x && int1.x > endPoint.x) || (int1.x < startPoint.x && int1.x < endPoint.x)){ intersection1Check = false; } // Checks if the intersection is within bounds
        if ( (int2.x > startPoint.x && int2.x > endPoint.x) || (int2.x < startPoint.x && int2.x < endPoint.x)){ intersection2Check = false; }


        // Select best point
        if (intersection1Check == true && intersection2Check == true){ // If 2 intersections return one closer to the end point
            
            if (distance(int1.x + xPos,int1.y + yPos,endPoint.x + xPos,endPoint.y + yPos) < distance(int2.x + xPos,int2.y + yPos,endPoint.x + xPos,endPoint.y + yPos)){ intersectionPoints.push_back(int1); }
            else{ intersectionPoints.push_back(int2); }
        }
        else if (intersection1Check == true && intersection2Check == false){ // If 1 intersections return unless end point is within look ahead dist
            
            if (pythagThisJohn(endPoint.x,endPoint.y) < lookAheadDis){ intersectionPoints.push_back(endPoint); }
            else{ intersectionPoints.push_back(int1); }
        }
        else if (intersection1Check == false && intersection2Check == true){ // If 1 intersections return unless end point is within look ahead dist
            
            if (pythagThisJohn(endPoint.x,endPoint.y) < lookAheadDis){ intersectionPoints.push_back(endPoint); }
            else{ intersectionPoints.push_back(int2); }
        }
        else{} // Don't append anything

        indexPP ++;
    }
    
    lcd::clear();
    lcd::print(0, "%f : x", intersectionPoints.back().x + xPos);
    lcd::print(1, "%f : y", intersectionPoints.back().y + yPos);
    lcd::print(2, "%f : tError", tErrorPP);
    lcd::print(3, "%f : lError", lErrorPP);
    lcd::print(4, "%f : xrob", xPos);
    lcd::print(5, "%f : yrob", yPos);
    lcd::print(6, "%f : right speed", rightPowPP);
    lcd::print(7, "%f : left speed", leftPowPP);
    

    if (intersectionPoints.size() != 0){ // Intersects path

        lastKnownIntersection = intersectionPoints.back();
        return intersectionPoints.back();
    }
    else {  // No intersections with path
    
        return lastKnownIntersection;
    }
}


float lErrPPkP = 300.0; // 100
float tErrPPkP = 15000.0; // 5000
float lDerPPkD = 1000.0;
float tDerPPkD = 400.0; // 100

float tDerPP = 0.0;
float lDerPP = 0.0;
float prevTErrorPP = 0.0;
float prevLErrorPP = 0.0;
float lPowPP = 0.0;
float tPowPP = 0.0;

void doThePurePursuit (vector<coord> path){

    runPP = 0;
    while (runPP < 50){

        robotPos.x = xPos;
        robotPos.y = yPos;

        followPoint = findBestIntersection(path, 30.0, robotPos); //? This is actually not a point but the difference in the robots position and the follow point

        tToTarget = arctan2(followPoint.x, followPoint.y); // Finds angle to target point
        tErrorPP = normAngle(tToTarget - (pi/2.0 - tPos)); // Find the difference in radians between target point and current theta in math radians
        lErrorPP = pythagThisJohn(followPoint.x, followPoint.y) * cos(tErrorPP); // Distance from the target scaled by the difference in angle

        tDerPP = tErrorPP - prevTErrorPP;
        lDerPP = lErrorPP - prevLErrorPP;

        lPowPP = lErrorPP * lErrPPkP + lDerPP * lDerPPkD; // Multiply each error by their tuning values
        tPowPP = tErrorPP * tErrPPkP + tDerPP * tDerPPkD;

        rightPowPP = lPowPP + tPowPP;
        leftPowPP = lPowPP - tPowPP; 

        if (fabs(rightPowPP) >= 12000.0 || fabs(leftPowPP) >= 12000.0){ // If power is over max value scale both sides
            if (fabs(rightPowPP) > fabs(leftPowPP)){
                rightPowPP = getDir(lPowPP + tPowPP) * 12000.0;
                leftPowPP = getDir(lPowPP - tPowPP) * fabs(12000.0 * (lPowPP - tPowPP) / (lPowPP + tPowPP));
            }
            else{
                rightPowPP = getDir(lPowPP + tPowPP) * fabs(12000.0 * (lPowPP + tPowPP) / (lPowPP - tPowPP));
                leftPowPP = getDir(lPowPP - tPowPP) * 12000.0;
            }
        }

        //rightDrive.move_voltage(rightPowPP); // Moves motors
        //leftDrive.move_voltage(leftPowPP);

        rightDrive.set_brake_modes(MOTOR_BRAKE_COAST);
        leftDrive.set_brake_modes(MOTOR_BRAKE_COAST);
        rightDrive.brake();
        leftDrive.brake();

        prevTErrorPP = tErrorPP;
        prevLErrorPP = lErrorPP;


        delay(100);
        /*
        if (pseudoVelocity < 0.25){ // Exit if robot hasnt moved position in a few loops
            runPP++;
        }
        else { runPP = 0; }
        */
    }
}

vector<float> coefficentsBC; // List of coefficents used for generating points (BC = Besier Curve)
vector<coord> output; // List of points in path
coord tempPoint; // Temporary variable to store each point value

vector<coord> bezierCurve (coord p1, coord p2, coord p3, coord p4, coord p5, int n){ // Inputs 5 points and how many segments to split curve into: //$ https://www.desmos.com/calculator/syvdhic9aw

    output.clear();

    for (int i = 0; i <= n; i++){ // Pushes back each coefficent
        coefficentsBC.push_back( to_float(i) / to_float(n) );
    }

    for (float w : coefficentsBC){

        tempPoint.x = (pow(w,4.0) * (p5.x - 4.0*p4.x + 6.0*p3.x - 4.0*p2.x + p1.x)) + (4.0*pow(w,3.0) * (p4.x - 3.0*p3.x + 3.0*p2.x - p1.x)) + (6.0*pow(w,2.0) * (p3.x - 2.0*p2.x + p1.x)) + (4.0*w * (p2.x - p1.x)) + p1.x;
        tempPoint.y = (pow(w,4.0) * (p5.y - 4.0*p4.y + 6.0*p3.y - 4.0*p2.y + p1.y)) + (4.0*pow(w,3.0) * (p4.y - 3.0*p3.y + 3.0*p2.y - p1.y)) + (6.0*pow(w,2.0) * (p3.y - 2.0*p2.y + p1.y)) + (4.0*w * (p2.y - p1.y)) + p1.y;
        output.push_back(tempPoint);
    }
    return(output);
}

void graphThePath (vector<coord> targetPath, vector<coord> actualPath){

    for (int i = 0; i < targetPath.size(); i++){
        //screen::draw_line();
    }
}
