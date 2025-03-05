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

vector<coord> actualPath {};

//! NOTEBOOK NOTES

//! 2. we calculated the distance to the endpoint wrong (intersection code rerturns distance from robot)
//! 3. it was using the behind point also, wasnt returning the end point of the function
//! 4. we only check the x-values of the intersections
//! 5. Robot was nacking at 0,0 so made path origin 0.01, 0.01
//! 6. Added max speed and made tuning values proportional to max distance
//! 7. Error in syncing odometry output with function loop IE pos was changing during the loop
//! 8. Added a fucntion that curbs speed when approchaing the end point


//! 7? removed delays from purepursuit and findbestintersection
//! 8? keep field centric coordinates and robot centric coordinates straight
//! 9? reformulated linear e slowdown to only drop to ssJ power (75%)
//! 10? made endpoint of entire path the follow point when it was inside lookahead
//! 11? 

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
    for (int i = 0; i <= actualPath.size() + 1; i++){
        screen::draw_pixel(inchToPixel(actualPath[i]).x,inchToPixel(actualPath[i]).y);
        delay(1);
    }
    while(controller.get_digital(DIGITAL_R1) == false){

        delay(100);
    }

}

coord findBestIntersection (vector<coord> path, float lookAheadDis, coord inputPoint){

    //lcd::set_text(7, std::to_string(2));

    shiftedPath.clear();

    for (coord point : path){
        //lcd::set_text(7, std::to_string(3));
        shiftedPath.push_back(coord(point.x - inputPoint.x, point.y - inputPoint.y)); // Shifts the point to put the robot position on the origin
    }

    indexPP = 0; // Lines distance along the path

    lastKnownIntersection = shiftedPath[0]; // Initializes beginning of path as last known intersection

    while(indexPP < shiftedPath.size() - 1){ // Runs loop for each pair of coordinates (each line)
        //lcd::set_text(7, std::to_string(4));

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

        if (powf(lookAheadDis, 2.0) * powf(R, 2.0) >= powf(D, 2.0)){
            //lcd::set_text(7, std::to_string(5));

            coord int1; // first possible intersection
            int1.x = (D * diffY + getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0); // Calculates intersection points
            int1.y = (-D * diffX + fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

            coord int2; // second possible intersection
            int2.x = (D * diffY - getDir(diffY) * diffX * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);
            int2.y = (-D * diffX - fabs(diffY) * sqrtf(powf(lookAheadDis, 2.0) * powf(R, 2.0) - powf(D, 2.0))) / powf(R, 2.0);

            
            if ( (int1.x+inputPoint.x > startPoint.x+inputPoint.x && int1.x+inputPoint.x > endPoint.x+inputPoint.x) || (int1.x+inputPoint.x < startPoint.x+inputPoint.x && int1.x+inputPoint.x < endPoint.x+inputPoint.x)){ intersection1Check = false; } // Checks if the intersection is within bounds
            if ( (int1.y+inputPoint.y > startPoint.y+inputPoint.y && int1.y+inputPoint.y > endPoint.y+inputPoint.y) || (int1.y+inputPoint.y < startPoint.y+inputPoint.y && int1.y+inputPoint.y < endPoint.y+inputPoint.y)){ intersection1Check = false; }

            if ( (int2.x+inputPoint.x > startPoint.x+inputPoint.x && int2.x+inputPoint.x > endPoint.x+inputPoint.x) || (int2.x+inputPoint.x < startPoint.x+inputPoint.x && int2.x+inputPoint.x < endPoint.x+inputPoint.x)){ intersection2Check = false; } // Checks if the intersection is within bounds
            if ( (int2.y+inputPoint.y > startPoint.y+inputPoint.y && int2.y+inputPoint.y > endPoint.y+inputPoint.y) || (int2.y+inputPoint.y < startPoint.y+inputPoint.y && int2.y+inputPoint.y < endPoint.y+inputPoint.y)){ intersection2Check = false; }    

            // Runs through checks to ensure intersection is on path
            if (isnan(int1.x) || isnan(int1.y)){intersection1Check = false;} // Check if there is an intersection
            if (isnan(int2.x) || isnan(int2.y)){intersection2Check = false;} // Check if there is an intersection

            //if (int1.x == int2.x && int1.y == int2.y){ intersection2Check = false; } // Check if they are the same point

            // Select best point
            if (intersection1Check == true && intersection2Check == true){ // If 2 intersections return one closer to the end point
                if (distance(int1.x+inputPoint.x, int1.y+inputPoint.y, endPoint.x+inputPoint.x, endPoint.y+inputPoint.y) <= distance(int2.x+inputPoint.x, int2.y+inputPoint.y, endPoint.x+inputPoint.x, endPoint.y+inputPoint.y)){ intersectionPoints.push_back(int1); }
                else{ intersectionPoints.push_back(int2); }
            }
            else if (intersection1Check == true && intersection2Check == false){ // If 1 intersections return unless end point is within look ahead dist
                if (distance(inputPoint.x, inputPoint.y, endPoint.x+inputPoint.x, endPoint.y+inputPoint.y) <= lookAheadDis){ intersectionPoints.push_back(endPoint); }
                else{ intersectionPoints.push_back(int1); }
            }
            else if (intersection1Check == false && intersection2Check == true){ // If 1 intersections return unless end point is within look ahead dist
                if (distance(inputPoint.x, inputPoint.y, endPoint.x+inputPoint.x, endPoint.y+inputPoint.y) <= lookAheadDis){ intersectionPoints.push_back(endPoint); }
                else{ intersectionPoints.push_back(int2); }
            }
            else{
                if(pythag(startPoint.x, startPoint.y) < lookAheadDis && pythag(endPoint.x, endPoint.y) < lookAheadDis){
                    intersectionPoints.push_back(endPoint);
                }
            } 
            //lcd::set_text(7, std::to_string(6));
        }

        indexPP ++;
        /*
        if (intersection1Check == true|| intersection2Check == true){
            lcd::clear();
            lcd::print(0, "%f : int1x", int1.x + inputPoint.x);
            lcd::print(1, "%f : int1y", int1.y + inputPoint.y);
            lcd::print(2, "%f : int2x", int2.x + inputPoint.x);
            lcd::print(3, "%f : int2y", int2.y + inputPoint.y);

            lcd::print(6, "%d : intcheck 1", intersection1Check);
            lcd::print(7, "%d : intcheck 2", intersection2Check);
        }
        */
        //delay(10);
        //lcd::set_text(7, std::to_string(7));
    }

    //std::cout << "int1Cheg: " << intersection1Check << "\n"; 
    //std::cout << "int2Cheg: " << intersection2Check << "\n"; 
    //std::cout << "intChoseX: " << intersectionPoints.back().x << "\n"; 
    //std::cout << "intChoseY: " << intersectionPoints.back().y << "\n"; 

    if (intersectionPoints.size() != 0){ // Intersects path
        lastKnownIntersection = intersectionPoints.back();
        return intersectionPoints.back();
    }
    else {  // No intersections with path
        return lastKnownIntersection;
    }
    //lcd::set_text(7, std::to_string(8));

}

float lErrPPkP = 1.0; //*tune
float tErrPPkP = 2.5; //*tune
float lDerPPkD = 0.0; //*tune
float tDerPPkD = 0.0; //*tune

float tDerPP = 0.0;
float lDerPP = 0.0;
float prevTErrorPP = 0.0;
float prevLErrorPP = 0.0;
float funch = 1'0'0'0'0'0'0'0'0;
float lPowPP = 0.0;
float tPowPP = 0.0;
float rightPowCalc = 0.0;
float leftPowCalc = 0.0;

float tWeightPP = 0.0;
float tWeightAPP = 0.0;
float tWeightBPP = 0.0;
float tWeightKPP = 0.25; //*tune
float distPP = 0.0;
float maxDistPP = 0.0;
bool funt = 1;
bool firstBoundary = 0;

float endpointDist = 0.0;
float slowScale = 0.0;
float slowScaleRad = 48.0; //*tune
float ssK = 0.1; //*tune
float ssJ = 0.0; //*tune
int ppLoopMax = 50; //*tune
bool pseudoVelSwitchPP = 0;
float pseudoVelLimitPP = 1.0; //*tune
float ppExitDist = 6.0; //*tune
float pseudoPP = 0.0;
int pploops = 0;
int graphedpoints = 0;
//* tune lookahead distance
//* tune speedcap

vector<string> graphingPoints {};

void purePursuit (vector<coord> path, float lookAheadDisPP, float speedCap, int ppSmooth){
    //lcd::set_text(7, std::to_string(0));

    runPP = 0;
    firstBoundary = 0;
    actualPath.clear();

    lErrPPkP *= (speedCap / lookAheadDisPP);
    tErrPPkP *= (speedCap / pi);

    while (runPP < ppLoopMax){
        //lcd::set_text(7, std::to_string(1));

        robotPos.x = xPos;
        robotPos.y = yPos;
        actualPath.push_back(robotPos);

        followPoint = findBestIntersection(path, lookAheadDisPP, robotPos); //robot centered
        distPP = pythag(followPoint.x, followPoint.y);

        pploops++;
        graphingPoints.push_back(to_string(deEed(followPoint.x + robotPos.x)) +  ", " + to_string(deEed(followPoint.y + robotPos.y)) + ", " + to_string(deEed(robotPos.x)) + ", " + to_string(deEed(robotPos.y)) + ", " + to_string(deEed(ppSmooth)) + ", " + to_string(pploops) + ", " + to_string(runPP));

        tToTarget = arctan2(followPoint.x, followPoint.y); // Finds angle to target point
        tErrorPP = normAngle(tToTarget - (pi/2.0 - tPos)); // Find the difference in radians between target point and current theta in math radians
        lErrorPP = distPP * cos(tErrorPP); // Distance from the target scaled by the difference in angle

        tDerPP = tErrorPP - prevTErrorPP;
        lDerPP = lErrorPP - prevLErrorPP;

        endpointDist = pythag(path.back().x - robotPos.x, path.back().y - robotPos.y);
        if(endpointDist < lookAheadDisPP && (ppSmooth == 0 || ppSmooth == 2)){
            if(firstBoundary == 0){
                maxDistPP = endpointDist;
                firstBoundary = 1;
            }
            tWeightAPP = (1.0 + sqrtf(1.0 + 4.0 / tWeightKPP)) / 2.0;
            tWeightBPP = 1.0 / (tWeightKPP * (tWeightAPP - 1.0));
            tWeightPP = 1.0 / (tWeightKPP * (1.0 - (distPP / maxDistPP) - tWeightAPP)) + tWeightBPP;
        }
        else{tWeightPP = 1.0;}

        lPowPP = lErrorPP * lErrPPkP + lDerPP * lDerPPkD; // Multiply each error by their tuning values
        tPowPP = tWeightPP * (tErrorPP * tErrPPkP + tDerPP * tDerPPkD);

        rightPowCalc = lPowPP + tPowPP;
        leftPowCalc = lPowPP - tPowPP;

        //lcd::set_text(7, std::to_string(9));

        if (fabs(rightPowPP) >= speedCap || fabs(leftPowPP) >= speedCap){ // If power is over max value scale both sides
            if (fabs(rightPowPP) > fabs(leftPowPP)){
                rightPowPP = getDir(rightPowCalc) * speedCap;
                leftPowPP = getDir(leftPowCalc) * fabs(speedCap * (leftPowCalc) / (rightPowCalc));
            }
            else if(fabs(rightPowPP) < fabs(leftPowPP)){
                rightPowPP = getDir(rightPowCalc) * fabs(speedCap * (rightPowCalc) / (leftPowCalc));
                leftPowPP = getDir(leftPowCalc) * speedCap;
            }
            else{
                rightPowPP = getDir(rightPowCalc) * speedCap;
                leftPowPP = getDir(leftPowCalc) * speedCap;
            }
        }
        else{
            rightPowPP = rightPowCalc;
            leftPowPP = leftPowCalc;
        }
        
        //lcd::clear();
        //lcd::print(0, "xPos: %f", robotPos.x);
        //lcd::print(1, "yPos: %f ", robotPos.y);
        //lcd::print(2, "tPos: %f", tPos);
        //lcd::print(3, "followPoint.x: %f", followPoint.x + robotPos.x);
        //lcd::print(4, "followPoint.y: %f", followPoint.y + robotPos.y);
        //lcd::print(5, "rightP: %f", rightPowPP);
        //lcd::print(6, "leftP: %f", leftPowPP);
        //lcd::print(7, "tWeightPP: %f", tWeightPP);
        
        //lcd::set_text(7, std::to_string(10));

        rightDrive.move_voltage(rightPowPP); // Moves motors
        leftDrive.move_voltage(leftPowPP);
        
        //rightDrive.set_brake_modes(MOTOR_BRAKE_COAST);
        //leftDrive.set_brake_modes(MOTOR_BRAKE_COAST);
        //rightDrive.brake();
        //leftDrive.brake();
        
        prevTErrorPP = tErrorPP;
        prevLErrorPP = lErrorPP;

        pseudoPP = pseudoVelocity;

        if(pseudoVelSwitchPP == 0 && pseudoPP > pseudoVelLimitPP){pseudoVelSwitchPP = 1;}
        
        //if(endpointDist < ppExitDist){runPP++;}
        if(endpointDist < ppExitDist || (pseudoVelSwitchPP == 1 && pseudoPP < pseudoVelLimitPP)){runPP++;}
        else{runPP = 0;}

        if(endpointDist < slowScaleRad && ppSmooth == 0){
            slowScale = 1.0 - (1.0 - ssJ) * (1.0 + ((powf(eConst, -ssK * endpointDist) - 1.0) / (1.0 - powf(eConst, -ssK * lookAheadDisPP))));
            rightPowPP *= slowScale;
            leftPowPP *= slowScale;
        }
        if(endpointDist < lookAheadDisPP && ppSmooth == 1){runPP = ppLoopMax;}
        if(endpointDist < 1.0 && ppSmooth == 2){runPP = ppLoopMax;}

        //if(controller.get_digital(DIGITAL_X) == true){runPP = ppLoopMax;}

        //lcd::set_text(7, std::to_string(11));

        rightDrive.move_voltage(rightPowPP);
        leftDrive.move_voltage(leftPowPP);

        delay(10);
    }
    if(ppSmooth == 0){drivetrain.brake();}
    lErrPPkP *= (lookAheadDisPP / speedCap);
    tErrPPkP *= (pi / speedCap);
}

vector<float> coefficentsBC; // List of coefficents used for generating points (BC = Bezier Curve)
coord tempPoint; // Temporary variable to store each point value

void bezierCurve(coord p1, coord p2, coord p3, coord p4, coord p5, int n, vector<coord>& listInput){ // Inputs 5 points and how many segments to split curve into: //$ https://www.desmos.com/calculator/syvdhic9aw

    for (int i = 0; i <= n; i++){ // Pushes back each coefficent
        coefficentsBC.push_back( to_float(i) / to_float(n) );
    }

    for (float w : coefficentsBC){

        tempPoint.x = (pow(w,4.0) * (p5.x - 4.0*p4.x + 6.0*p3.x - 4.0*p2.x + p1.x)) + (4.0*pow(w,3.0) * (p4.x - 3.0*p3.x + 3.0*p2.x - p1.x)) + (6.0*pow(w,2.0) * (p3.x - 2.0*p2.x + p1.x)) + (4.0*w * (p2.x - p1.x)) + p1.x;
        tempPoint.y = (pow(w,4.0) * (p5.y - 4.0*p4.y + 6.0*p3.y - 4.0*p2.y + p1.y)) + (4.0*pow(w,3.0) * (p4.y - 3.0*p3.y + 3.0*p2.y - p1.y)) + (6.0*pow(w,2.0) * (p3.y - 2.0*p2.y + p1.y)) + (4.0*w * (p2.y - p1.y)) + p1.y;
        listInput.push_back(tempPoint);
    }
}

int whatToRun = 0;
coord RCDCPos(0.0,0.0);
void runCodeDuringCode (){

    while(1){
        RCDCPos.x = xPos;
        RCDCPos.y = yPos;

        if (whatToRun == 1 && distance(RCDCPos.x, RCDCPos.y, 36.0, 84.0) <= 6.0){ // You set the whatToRun variable before a PP or toPoint and then it'll run this when you want it
            intakePiston.set_value(1);
            whatToRun = 0; // Resets at the end of the loop so it doesnt accidentially run twice
        }
        else if(whatToRun == 2){
            
            whatToRun = 0;
        }
        else if(whatToRun == 3){
            
            whatToRun = 0;
        }
        else if(whatToRun == 4){
            
            whatToRun = 0;
        }

        delay(20);
    }

}