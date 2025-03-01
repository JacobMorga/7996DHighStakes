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

vector<coord> acutalPath {};

//! NOTEBOOK NOTES
//! 1. IT CANT MOVE TO A POINT DIRECTLY AHEAD - VALUES OSCILATE BETWEEN 100M - -500M
//! 2. we calculated the distance to the endpoint wrong (intersection code rerturns distance from robot)
//! 3. it was using the behind point also, wasnt returning the end point of the function
//! 4. we only check the x-values of the intersections
//! 5. Robot was nacking at 0,0 so made path origin 0.01,0.01

coord findBestIntersection (vector<coord> path, float lookAheadDis, coord inputPoint){

    shiftedPath.clear();

    for (coord point : path){

        shiftedPath.push_back(coord(point.x - xPos, point.y - yPos)); // Shifts the point to put the robot position on the origin
        delay(5);
    }

    indexPP = 0; // Lines distance along the path

    lastKnownIntersection = shiftedPath[0]; // Initializes beginning of path as last known intersection

    while(indexPP < shiftedPath.size() - 1){ // Runs loop for each pair of coordinates (each line)

        intersection1Check = true;  // Resets intersection checks
        intersection2Check = true; 

        coord startPoint;
        startPoint.x = shiftedPath[indexPP].x; // Retrives x and y for each end point of line
        startPoint.y = shiftedPath[indexPP].y;

        coord endPoint;
        endPoint.x = shiftedPath[indexPP + 1].x;
        endPoint.y = shiftedPath[indexPP + 1].y;

        diffX = endPoint.x - startPoint.x;
        diffY = endPoint.y - startPoint.y;
        R = distance(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        D = startPoint.x * endPoint.y - endPoint.x * startPoint.y;

        coord int1; // first possible intersection
        int1.x = (D * diffY + getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0); // Calculates intersection points
        int1.y = (-D * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

        coord int2; // second possible intersection
        int2.x = (D * diffY - getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);
        int2.y = (-D * diffX - fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

        
        if ( (int1.x > startPoint.x && int1.x > endPoint.x) || (int1.x < startPoint.x && int1.x < endPoint.x)){ intersection1Check = false; } // Checks if the intersection is within bounds
        if ( (int1.y > startPoint.y && int1.y > endPoint.y) || (int1.y < startPoint.y && int1.y < endPoint.y)){ intersection1Check = false; }

        if ( (int2.y > startPoint.y && int2.y > endPoint.y) || (int2.y < startPoint.y && int2.y < endPoint.y)){ intersection2Check = false; }
        if ( (int2.x > startPoint.x && int2.x > endPoint.x) || (int2.x < startPoint.x && int2.x < endPoint.x)){ intersection2Check = false; }
        

        // Runs through checks to ensure intersection is on path
        if (isnan(int1.x) || isnan(int1.y)){intersection1Check = false;} // Check if there is an intersection
        if (isnan(int2.x) || isnan(int2.y)){intersection2Check = false;} // Check if there is an intersection

        //if (int1.x == int2.x && int1.y == int2.y){ intersection2Check = false; } // Check if they are the same point

        // Select best point
        if (intersection1Check == true && intersection2Check == true){ // If 2 intersections return one closer to the end point
            
            if (distance(int1.x+xPos, int1.y+yPos, endPoint.x+xPos, endPoint.y+yPos) <= distance(int2.x+xPos, int2.y+yPos, endPoint.x+xPos, endPoint.y+yPos)){ intersectionPoints.push_back(int1); }
            else{ intersectionPoints.push_back(int2); }
        }
        else if (intersection1Check == true && intersection2Check == false){ // If 1 intersections return unless end point is within look ahead dist
            
            if (distance(xPos, yPos, endPoint.x+xPos, endPoint.y+yPos) <= lookAheadDis){ intersectionPoints.push_back(endPoint); }
            else{ intersectionPoints.push_back(int1); }
        }
        else if (intersection1Check == false && intersection2Check == true){ // If 1 intersections return unless end point is within look ahead dist
            
            if (distance(xPos, yPos, endPoint.x+xPos, endPoint.y+yPos) <= lookAheadDis){ intersectionPoints.push_back(endPoint); }
            else{ intersectionPoints.push_back(int2); }
        }
        else{} // Don't append anything

        indexPP ++;
        /*
        if (intersection1Check == true|| intersection2Check == true){
            lcd::clear();
            lcd::print(0, "%f : int1x", int1.x + xPos);
            lcd::print(1, "%f : int1y", int1.y + yPos);
            lcd::print(2, "%f : int2x", int2.x + xPos);
            lcd::print(3, "%f : int2y", int2.y + yPos);

            lcd::print(6, "%d : intcheck 1", intersection1Check);
            lcd::print(7, "%d : intcheck 2", intersection2Check);
        }
            */
            
        
        delay(10);
    }

    //std::cout << "int1Cheg: " << intersection1Check << "\n"; 
    //std::cout << "int2Cheg: " << intersection2Check << "\n"; 
    //std::cout << "intChoseX: " << intersectionPoints.back().x << "\n"; 
    //std::cout << "intChoseX: " << intersectionPoints.back().y << "\n"; 

    if (intersectionPoints.size() != 0){ // Intersects path

        lastKnownIntersection = intersectionPoints.back();
        return intersectionPoints.back();
    }
    else {  // No intersections with path
    
        return lastKnownIntersection;
    }
}

float lookaheadinputvariable = 18.0;
float lErrPPkP = 4500.0 / lookaheadinputvariable; //!300.0; //! why is this on the linear
float tErrPPkP = 15000.0; // 5000
float lDerPPkD = 0.0; //actually set in line 158 if statement
float tDerPPkD = 5000.0; //1000.0; //400.0; // 100

float tDerPP = 0.0;
float lDerPP = 0.0;
float prevTErrorPP = 0.0;
float prevLErrorPP = 0.0;
float lPowPP = 0.0;
float tPowPP = 0.0;

float tWeightPP = 0.0;
float tWeightAPP = 0.0;
float tWeightBPP = 0.0;
float tWeightKPP = 0.25;
float distPP = 0.0;
float maxDistPP = 0.0;
bool firstBoundary = 0;

void doThePurePursuit (vector<coord> path){

    runPP = 0;
    firstBoundary = 0;

    acutalPath.clear();

    while (runPP < 50){

        robotPos.x = xPos;
        robotPos.y = yPos;

        acutalPath.push_back(robotPos);

        followPoint = findBestIntersection(path, lookaheadinputvariable, robotPos); //? This is actually not a point but the difference in the robots position and the follow point
        distPP = pythag(followPoint.x, followPoint.y);

        //std::cout << followPoint.x + xPos << ", " << followPoint.y + yPos << ", " << xPos << ", " << yPos << "\n";

        tToTarget = arctan2(followPoint.x, followPoint.y); // Finds angle to target point
        tErrorPP = normAngle(tToTarget - (pi/2.0 - tPos)); // Find the difference in radians between target point and current theta in math radians
        lErrorPP = pythag(followPoint.x, followPoint.y) * cos(tErrorPP); // Distance from the target scaled by the difference in angle

        tDerPP = tErrorPP - prevTErrorPP;
        lDerPP = lErrorPP - prevLErrorPP;
        /*
        if(distPP < lookaheadinputvariable){//
            if(firstBoundary == 0){
                maxDistPP = distPP;
                firstBoundary = 1;
            }
            lDerPPkD = 1'000'000.0; //500'000 before
            tWeightAPP = (1.0 + sqrt(1.0 + 4.0 / tWeightKPP)) / 2.0;
            tWeightBPP = 1.0 / (tWeightKPP * (tWeightAPP - 1.0));
            tWeightPP = 1.0 / (tWeightKPP * (1.0 - (distPP / maxDistPP) - tWeightAPP)) + tWeightBPP;
        }
        else{
            lDerPPkD = 0.0;
            tWeightPP = 1.0;
        }
        */


        lPowPP = lErrorPP * lErrPPkP + lDerPP * lDerPPkD; // Multiply each error by their tuning values
        tPowPP = tErrorPP * tErrPPkP + tDerPP * tDerPPkD; //tWeightPP * (tErrorPP * tErrPPkP + tDerPP * tDerPPkD);

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

        //lcd::print(4, "%f : right", rightPowPP);
        //lcd::print(5, "%f : leftP", leftPowPP);

        rightDrive.move_voltage(rightPowPP); // Moves motors
        leftDrive.move_voltage(leftPowPP);

        
        //rightDrive.set_brake_modes(MOTOR_BRAKE_COAST);
        //leftDrive.set_brake_modes(MOTOR_BRAKE_COAST);
        //rightDrive.brake();
        //leftDrive.brake();
        
        
        prevTErrorPP = tErrorPP;
        prevLErrorPP = lErrorPP;

        delay(10);
        /*
        if (pseudoVelocity < 0.25){ // Exit if robot hasnt moved position in a few loops
            runPP++;
        }
        else { runPP = 0; }
        */

        if (controller.get_digital(DIGITAL_X) == true){
            runPP = 600;
        }
    }
    drivetrain.set_brake_modes(MOTOR_BRAKE_COAST);
    drivetrain.brake();

    graphThePath(path,acutalPath);
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

coord inchToPixel(coord input){
    float XcenterRelToFieldCenter = 48.0;
    float YcenterRelToFieldCenter = -48.0;

    return coord((input.x + XcenterRelToFieldCenter) / 144.0 * 240.0 + 240.0, (input.y + YcenterRelToFieldCenter) / 144.0 * 240.0 + 120.0);
}

void graphThePath (vector<coord> targetPath, vector<coord> actualPath){
    screen::set_eraser(COLOR_BLACK);
    //lcd::clear();
    screen::erase();
    screen::set_pen(COLOR_LIGHT_GOLDENROD_YELLOW);

    delay(10);

    screen::draw_rect(120.0,0.0,240.0,120.0);
    screen::draw_rect(240.0,0.0,360.0,120.0);
    screen::draw_rect(120.0,120.0,240.0,240.0);
    screen::draw_rect(240.0,120.0,360.0,240.0);

    screen::set_pen(COLOR_BLUE);
    for (int i = 0; i <= targetPath.size(); i++){
        screen::draw_line(inchToPixel(targetPath[i]).x,inchToPixel(targetPath[i]).y,inchToPixel(targetPath[i+1]).x,inchToPixel(targetPath[i+1]).y);
        delay(1);
    }

    screen::set_pen(COLOR_RED);
    for (int i = 0; i <= actualPath.size(); i++){
        screen::draw_line(inchToPixel(actualPath[i]).x,inchToPixel(actualPath[i]).y,inchToPixel(actualPath[i+1]).x,inchToPixel(actualPath[i+1]).y);
        delay(1);
    }
    while(controller.get_digital(DIGITAL_R1) == false){

        delay(100);
    }

}
