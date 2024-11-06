#include "main.h"

vector<vector<float>> intersectionPoints {};

float px1 = 0.0;
float py1 = 0.0;
float px2 = 0.0;
float py2 = 0.0;

float diffX = 0.0;
float diffY = 0.0;
float diffR = 0.0;
float diffD = 0.0;

float intersectionCount = 0.0;
int counter = 0;

float intX1 = 0.0;
float intX2 = 0.0;
float intY1 = 0.0;
float intY2 = 0.0;
    
vector<vector<float>> circlePathIntersection (vector<vector<float>> straightLinePath, float robotX, float robotY, float lookAheadDis){

    // move the robot center to the origin (easier math)
    for (vector<float> point : straightLinePath){

        point[0] -= robotX;
        point[1] -= robotY;
    }

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

            intX1 = (diffD * diffY + fabs(diffY) / diffY * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
            intX2 = (diffD * diffY - fabs(diffY) / diffY * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);

            intY1 = (-diffD * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);
            intY2 = (-diffD * diffX - fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(diffR, 2.0) - powf(diffD, 2.0))) / powf(diffR, 2.0);


            if (intX1 == intX2 && intY1 == intY2){

                if (!(intX1 > px1 && intX1 > px2) && !(intX1 < px1 && intX1 < px2)){
                    intersectionPoints.push_back({intX1, intY1, static_cast<float>(counter)});
                }
            }
            else {
                if (!(intX1 > px1 && intX1 > px2) && !(intX1 < px1 && intX1 < px2)){
                    intersectionPoints.push_back({intX1, intY1, static_cast<float>(counter)});
                }

                if (!(intX2 > px1 && intX2 > px2) && !(intX2 < px1 && intX2 < px2)){
                    intersectionPoints.push_back({intX2, intY2, static_cast<float>(counter)});
                }
            }
        }
        counter++;
    }

    return intersectionPoints;
}

vector<float> determineBestPoint (vector<vector<float>> intersections, vector<vector<float>> path){

    vector<float> lastP = intersections[intersections.size() - 1];
    vector<float> secLastP = intersections[intersections.size() - 2];

    if (lastP[2] > secLastP[2]){

        return {lastP[0], lastP[1]};
    }
    else{

        vector<float> lineEP = path[lastP[2] + 1];

        float distLP = sqrtf(powf(lineEP[0] - lastP[0], 2.0) + powf(lineEP[1] - lastP[1], 2.0));
        float distSLP = sqrtf(powf(lineEP[0] - secLastP[0], 2.0) + powf(lineEP[1] - secLastP[1], 2.0));

        if (distLP > distSLP){
            return {secLastP[0], secLastP[1]};
        }
        else{
            return {lastP[0], lastP[1]};
        }
    }
}


vector<vector<float>> path {{-9.0,9.6},{-5.9,-6.4},{0.66,6.84},{0.0,5.0},{6.0,2.78}};
void PurePursuit (){

    determineBestPoint( circlePathIntersection(path, xPos, yPos, 10.0) , path );
}


