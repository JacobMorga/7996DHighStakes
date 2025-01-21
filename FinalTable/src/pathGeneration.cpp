#include "main.h"

using namespace std;
float initialSlope = 0.0;
vector<coordinate> segmentedArc (coordinate previousPoint, coordinate startPoint, coordinate endPoint, int segmentCount){
    if(startPoint.x == previousPoint.x){initialSlope = 100000.0;}
    else{initialSlope = (startPoint.y - previousPoint.y) / (startPoint.x - previousPoint.x);}
    
}