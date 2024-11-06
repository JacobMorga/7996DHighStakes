#include "iostream"
using namespace std;



float x1 = 1.0;
float y1w = 4.0;

float x2 = 1.3;
float y2 = 3;

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

    if (val < 0){return -1.0;}
    else {return 1.0;}
}

int main (){

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
    

    return 1;
}