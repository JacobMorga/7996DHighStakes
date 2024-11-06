#include "iostream"
#include "vector"
#include "math.h"
using namespace std;



float x1 = 1.0;
float y1w = 4.0;

float x2 = 1.3;
float y2 = 3.0;

float rad = 3.0;

float xPos = 0.0;
float yPos = 0.0;

float diffX = 0.0;
float diffY = 0.0;
float diffR = 0.0;
float diffD = 0.0;

float intersectionCount = 0.0;

float intX1 = 0.0;
float intX2 = 0.0;
float intY1 = 0.0;
float intY2 = 0.0;

float negorno (float val){

    if (val < 0.0){return -1.0;}
    else {return 1.0;}
}
/*
void funchinfunction (){

    x1 -= xPos;
    y1w -= yPos;
    x2 -= xPos;
    y2 -= yPos;

    diffX = x2-x1;
    diffY = y2-y1w;

    diffR = sqrtf(powf(diffX, 2.0) + powf(diffY, 2.0));
    diffD = x1*y2 - x2*y1w;

    intersectionCount = powf(rad, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0);

    if (intersectionCount > 0.0){
        cout << "two intersections" << "\n";

        intX1 = (diffD * diffY + negorno(diffY) * diffX * sqrtf(powf(rad, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
        intX2 = (diffD * diffY - negorno(diffY) * diffX * sqrtf(powf(rad, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);

        intY1 = (-diffD * diffX + fabs(diffY) * sqrtf(powf(rad, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
        intY2 = (-diffD * diffX - fabs(diffY) * sqrtf(powf(rad, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);

        if (x2 > intX1 > x1 && y1w > intY1 > y2 ){
            cout << "intersection X1 = " << intX1 << "\n";
            cout << "intersection Y1 = " << intY1 << "\n";
        }
        else {cout << "point 1 failed" << "\n";}

        if (x2 > intX2 > x1 && y1w > intY2 > y2 ){
            cout << "intersection X2 = " << intX2 << "\n";
            cout << "intersection Y2 = " << intY2 << "\n";
        }
        else {cout << "point 2 failed" << "\n";}

    }
    else if (intersectionCount < 0.0){

        cout << "no intersections";
    }
    else{
        cout << "tangent";
    }
    
}
*/


vector<vector<float>> path {{-2.8, 2.9},{-1.4, -3.3},{-0.59,3.77},{3.0,-5.0},{3.0,4.01}};
vector<vector<float>> intersectionPoints {};



float px1 = 0.0;
float py1 = 0.0;
float px2 = 0.0;
float py2 = 0.0;

int counter = 0;

float currentX = 0.0;
float currentY = 0.0;
double lookAheadRadius = 3.0;
    
void funchininerfunction (vector<vector<float>> straightLinePath, float robotX, float robotY, float lookAheadDis){

    // move the robot center to the origin (easier math)
    for (vector<float> point : straightLinePath){

        point[0] -= robotX;
        point[1] -= robotY;

        cout << "(" << point[0] << ", " << point[1] << ")   ";

    }

    cout << "\n";

    while (counter < straightLinePath.size() - 1){

        px1 = straightLinePath[counter][0];
        py1 = straightLinePath[counter][1];
        px2 = straightLinePath[counter + 1][0];
        py2 = straightLinePath[counter + 1][1];

        // begining of calculations
        diffX = px2-px1;
        diffY = py2-py1;

        diffR = sqrtf(powf(diffX, 2.0) + powf(diffY, 2.0));
        diffD = px1*py2 - px2*py1;

        intersectionCount = powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0);

        if (intersectionCount >= 0.0){ // One or two intersections
            cout << "two intersections" << "\n";

            intX1 = (diffD * diffY + negorno(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
            intX2 = (diffD * diffY - negorno(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);

            intY1 = (-diffD * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
            intY2 = (-diffD * diffX - fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);


            if (intX1 == intX2 && intY1 == intY2){

                if ((intX1 > px1 && intX1 > px2) || (intX1 < px1 && intX1 < px2)){
                    cout << "point 1 failed" << "\n";
                }
                else {

                    intersectionPoints.push_back({intX1, intY1});
                }
            }
            else {
                if ((intX1 > px1 && intX1 > px2) || (intX1 < px1 && intX1 < px2)){
                    cout << "point 1 failed" << "\n";
                    cout << "intersection X1 = " << intX1 << "   " << px1 << "   " << "\n";
                    cout << "intersection Y1 = " << intY1 << "   " << px2 << "   " << "\n";
                }
                else {
                    intersectionPoints.push_back({intX1, intY1});
                }

                if ((intX2 > px1 && intX2 > px2) || (intX2 < px1 && intX2 < px2)){
                    cout << "point 2 failed" << "\n";
                    cout << "intersection X2 = " << intX2 << "   " << px1 << "   " << "\n";
                    cout << "intersection Y2 = " << intY2 << "   " << px2 << "   " << "\n";
                }
                else {
                    intersectionPoints.push_back({intX2, intY2});
                }
            }
        }
        else{ // No intersections

            cout << "no intersections" << "\n";
        }
        counter++;
    }
    for (vector<float> point : intersectionPoints){

        for (float item : point){
            cout << item << ", ";
        }
        cout << "\n";
    }
}


int main (){

    funchininerfunction(path, currentX, currentY, lookAheadRadius);

    return 1;
}