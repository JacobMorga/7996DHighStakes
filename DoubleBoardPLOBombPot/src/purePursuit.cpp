#include "main.h"
using namespace std;
/*
int counter;
vector<vector<float>> shiftedPath = {};
vector<vector<float>> intersectionPoints = {};
float x1,y1,x2,y2;
float intersectionX1,intersectionY1,intersectionX2,intersectionY2;
float diffX,diffY,R,D;

float lookAheadDis = 8.0;
float intersectionCount;

bool intersection1Check, intersection2Check = true;

vector<float> bestIntersection (vector<vector<float>> path){

    shiftedPath.clear();

    for (vector<float> point : path){

        shiftedPath.push_back({point[0] - xPos, point[1] - yPos}); // Shifts the point to put the robot on the origin
    }

    counter = 0;

    intersectionPoints.clear(); // Erases points

    while(counter < shiftedPath.size() - 1){ // Checks intersections for each pair of lines

        intersection1Check = true;  // All good on intersection checks
        intersection2Check = true;

        x1 = shiftedPath[counter][0]; // Retrives x and y for each point and the next one
        y1 = shiftedPath[counter][1];
        x2 = shiftedPath[counter + 1][0];
        y2 = shiftedPath[counter + 1][1];

        diffX = x2-x1;
        diffY = y2-y1;

        R = distance(x1,y1,x2,y2);
        D = x1*y2 - x2*y1;

        intersectionX1 = (D * diffY + getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0); // Calculates intersection points
        intersectionY1 = (-D * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

        intersectionX2 = (D * diffY - getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);
        intersectionY2 = (-D * diffX - fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

        // Runs through checks and appends best intersection last

        if (isnan(intersectionX1) || isnan(intersectionY1)){ intersection1Check = false;} // Check if there is an intersection
        if (isnan(intersectionX2) || isnan(intersectionY2)){ intersection2Check = false;}

        if (intersectionX1 == intersectionX2 && intersectionY1 == intersectionY2){ intersection2Check = false; } // Check if they are the same point

        if ( (intersectionX1 > x1 && intersectionX1 > x2) || (intersectionX1 < x1 && intersectionX1 < x2)){ intersection1Check = false; } // Checks if the intersection is within bounds
        if ( (intersectionX2 > x1 && intersectionX2 > x2) || (intersectionX2 < x1 && intersectionX2 < x2)){ intersection2Check = false; }

        // Return best point
        if (intersection1Check == true && intersection2Check == true){ // If two intersections then append one closer to end of line
            if (distance(intersectionX1,intersectionY1,x2,y2) < distance(intersectionX2,intersectionY2,x2,y2)){ 
                intersectionPoints.push_back({intersectionX1, intersectionY1}); 
            }
            else { intersectionPoints.push_back({intersectionX2, intersectionY2}); }
        }
        else if (intersection1Check == true && intersection2Check == false){ // If only one intersection check if end point is in lookahead distance
            if (distance(intersectionX1,intersectionY1,x2,y2) < distance(xPos,yPos,x2,y2)){ 
                intersectionPoints.push_back({intersectionX1, intersectionY1}); 
            }
            else { intersectionPoints.push_back({x2, y2}); }
        }
        else if (intersection1Check == false && intersection2Check == true){ // If only one intersection check if end point is in lookahead distance 
            if (distance(intersectionX2,intersectionY2,x2,y2) < distance(xPos,yPos,x2,y2)){ 
                intersectionPoints.push_back({intersectionX2, intersectionY2}); 
            }
            else { intersectionPoints.push_back({x2, y2}); }
        }
        else{ // If no intersections check if endpoint is within lookahead distance
            if (distance(xPos,yPos,x2,y2) < lookAheadDis){
                intersectionPoints.push_back({x2, y2});
            }
        }

        counter++;
    }

    return intersectionPoints.back(); // Returns last point in list
}
*/