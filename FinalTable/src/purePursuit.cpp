#include "main.h"
using namespace std;
/*
int index;
vector<coord> shiftedPath = {};
vector<coord> intersectionPoints = {};

float diffX,diffY,R,D;

float lookAheadDis = 8.0;
float int1Dist,int2Dist = 0.0;
bool intersection1Check, intersection2Check = true;
coord lastKnownIntersection (0.0,0.0);

coord findBestIntersection (vector<coord> path){

    shiftedPath.clear();

    for (coord point : path){

        shiftedPath.push_back(coord(point.x - xPos, point.y - yPos)); // Shifts the point to put the robot position on the origin
    }

    index = 0; // Lines distance along the path


    while(index < shiftedPath.size() - 1){ // Runs loop for each pair of coordinates (each line)

        intersection1Check = true;  // Resets intersection checks
        intersection2Check = true;

        coord startPoint;
        startPoint.x = shiftedPath[index].x; // Retrives x and y for each end point of line
        startPoint.y = shiftedPath[index].y; 

        coord endPoint;
        endPoint.x = shiftedPath[index + 1].x;
        endPoint.y = shiftedPath[index + 1].y;

        diffX = endPoint.x-startPoint.x;
        diffY = endPoint.y-startPoint.y;
        R = distance(startPoint.x,startPoint.y,endPoint.x,endPoint.y);
        D = startPoint.x*endPoint.y - endPoint.x*startPoint.y;

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
            
            if (distance(int1.x,int1.y,endPoint.x,endPoint.y) < distance(int2.x,int2.y,endPoint.x,endPoint.y)){ intersectionPoints.push_back(int1); }
            else{ intersectionPoints.push_back(int2); }
        }
        else if (intersection1Check == true && intersection2Check == false){ // If 1 intersections return unless end point is within look ahead dist
            
            if (distance(xPos,yPos,endPoint.x,endPoint.y) < lookAheadDis){ intersectionPoints.push_back(endPoint); }
            else{ intersectionPoints.push_back(int1); }
        }
        else if (intersection1Check == false && intersection2Check == true){ // If 1 intersections return unless end point is within look ahead dist
            
            if (distance(xPos,yPos,endPoint.x,endPoint.y) < lookAheadDis){ intersectionPoints.push_back(endPoint); }
            else{ intersectionPoints.push_back(int2); }
        }
        else{} // Don't append anything

        index ++;
    }

    if (intersectionPoints.size() != 0){ // Intersects path

        lastKnownIntersection = intersectionPoints.back();
        return intersectionPoints.back();
    }
    else {  // No intersections with path
    
        return lastKnownIntersection;
    }
}

float PPkp = 0.0;
float PPtkp = 0.0;

int runPP = 0;
float tToTarget = 0.0;
float tError,lError = 0.0;
float rightPow,leftPow = 0.0;

coord followPoint;
void doThePurePursuit (coord followPoint, vector<coord> path){

    runPP = 0;
    while (runPP < 50){

        followPoint = findBestIntersection(path); //? This is actually not a point but the difference in the robots position and the follow point

        tToTarget = arctan2(followPoint.x, followPoint.y); // Finds angle to target point
        tError = normAngle(tToTarget - (pi/2.0 - tPos)); // Find the difference in radians between target point and current theta in math radians

        lError = pythagThisJohn(followPoint.x, followPoint.y) * cos(tError); // Distance from the target scaled by the difference in angle

        rightPow = lError * PPkp + tError * PPtkp; // Multiply each error by their tuning values
        leftPow = lError * PPkp - tError * PPtkp;

        if (fabs(rightPow) >= 12000.0|| fabs(leftPow) >= 12000.0){ // If power is over max value scale both sides
            if (fabs(rightPow) > fabs(leftPow)){
                rightPow = getDir(rightPow) * 12000.0;
                leftPow = getDir(leftPow) * fabs(12000.0 * (leftPow) / (rightPow));
            }
            else{
                rightPow = getDir(rightPow) * fabs(600.0 * (rightPow) / (leftPow));
                leftPow = getDir(leftPow) * 600.0;
            }
        }

        rightDrive.move_voltage(rightPow); // Moves motors
        leftDrive.move_voltage(leftPow);

        delay(10);

        if (pseudoVelocity < 0.25){ // Exit if robot hasnt moved position in a few loops
            runPP++;
        }
        else { runPP = 0; }
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
*/