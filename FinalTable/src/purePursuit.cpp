#include "main.h"
using namespace std;

int index;
vector<coordinate> shiftedPath = {};
vector<coordinate> intersectionPoints = {};

float diffX,diffY,R,D;

float lookAheadDis = 8.0;
float intersectionCount;
float int1Dist,int2Dist;

bool intersection1Check, intersection2Check = true;

coordinate findBestIntersection (vector<coordinate> path){

    shiftedPath.clear();

    for (coordinate point : path){

        coordinate shiftedPoint;
        shiftedPoint.x = point.x - xPos;
        shiftedPoint.y = point.y - yPos;

        shiftedPath.push_back(shiftedPoint); // Shifts the point to put the robot position on the origin
    }

    index = 0; // Lines distance along the path

    intersectionPoints.clear(); // Erases points

    while(index < shiftedPath.size() - 1){ // Runs loop for each pair of coordinates (each line)

        intersection1Check = true;  // All good on intersection checks
        intersection2Check = true;

        coordinate startPoint;
        startPoint.x = shiftedPath[index].x; // Retrives x and y for each end point 
        startPoint.y = shiftedPath[index].y; 

        coordinate endPoint;
        endPoint.x = shiftedPath[index + 1].x;
        endPoint.y = shiftedPath[index + 1].y;

        diffX = endPoint.x-startPoint.x;
        diffY = endPoint.y-startPoint.y;
        R = distance(startPoint.x,startPoint.y,endPoint.x,endPoint.y);
        D = startPoint.x*endPoint.y - endPoint.x*startPoint.y;

        coordinate int1; // first possible intersection
        int1.x = (D * diffY + getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0); // Calculates intersection points
        int1.y = (-D * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

        coordinate int2; // second possible intersection
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

    return intersectionPoints.back();
}

bool runPP = true;
float tToTarget = 0.0;
float tError = 0.0;
float lError = 0.0;
coordinate followPoint;
void doThePurePursuit (coordinate followPoint, vector<coordinate> path){

    while (runPP == true){

        followPoint = findBestIntersection(path); //? This is actually not a point but the difference in the robots position and the follow point

        tToTarget = arctan2(followPoint.x, followPoint.y); // Finds angle to target point
        tError = normAngle(tToTarget - (pi/2.0 - tPos)); // Find the difference in radians between target point and current theta in math radians

        lError = pythagThisJohn(followPoint.x, followPoint.y) * cos(tError); // Distance from the target scaled by the difference in angle

        

    }

}