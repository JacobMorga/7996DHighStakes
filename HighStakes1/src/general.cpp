#include "main.h"
using namespace std;

float getDir(float input){
    if (input > 0){return 1;}
    else if (input < 0){return -1;}
    else{return 0;}
}

float dif12 = 0.0;
float dif23 = 0.0;
float dif13 = 0.0;
float leastdif = 0.0;
int difselect = 0;
float angle = 0.0;

const bool on = true;
const bool off = false;
const bool pressed = true;
const bool unpressed = false;

float getAngle(void){
    dif12 = fabs(inertial1.get_rotation() - inertial2.get_rotation());
    dif23 = fabs(inertial2.get_rotation() - inertial3.get_rotation());
    dif13 = fabs(inertial1.get_rotation() - inertial3.get_rotation());

    leastdif = dif12; difselect = 1;
    if (dif23 <= leastdif){leastdif = dif23; difselect = 2;}
    if (dif13 <= leastdif){leastdif = dif13; difselect = 3;}
    
    if (difselect == 1){angle = 0.5 * (inertial1.get_rotation() + inertial2.get_rotation());}
    else if (difselect == 2){angle = 0.5 * (inertial2.get_rotation() + inertial3.get_rotation());}
    else{angle = 0.5 * (inertial1.get_rotation() + inertial3.get_rotation());}

    return angle;
}

float getRangle(void){
    return pi / 180.0 * getAngle();
}

float arctan2(float x, float y){
    float a = 0.0;
    if (x > 0){
        a = (atan(y / x));
    }
    else if (x < 0){
        a = (atan(y / x) + pi);
    }
    else if (x == 0){
        if (y > 0){
            a = (pi / 2.0);
        }
        if (y < 0){
            a = (1.5 * pi);
        }
    }
    return a;
}



float newCord = 0;
DisplayBox::DisplayBox (float inX1, float inY1, float inX2, float inY2, float inAmtX, float inAmtY){

    X1 = inX1;
    Y1 = inY1;
    X2 = inX2;
    Y2 = inY2;
    AmtX = inAmtX;
    AmtY = inAmtY;
}
void DisplayBox::setButtons (vector<int> inColors, bool inMultiSelect){

    buttonColors = inColors;
    multiSelect = inMultiSelect;
    hasButtons = true;
}
void DisplayBox::drawVerticalLines (){
    for (int h = 1; h < AmtY; h++){

        newCord = Y1 + (fabs(Y1-Y2) / AmtY) * h;
        screen::draw_line(X1,newCord,X2,newCord);
    }
}
void DisplayBox::drawHorizontalLines (){
    for (int z = 1; z < AmtX; z++){

        newCord = X1 + (fabs(X1-X2) / AmtX) * z;
        screen::draw_line(newCord,Y1,newCord,Y2);
    }
}
void DisplayBox::handleButtons (){
    
}

void autonSelectorNEW (vector<DisplayBox> divisions){
    while (1){
        screen::erase();
        delay(5);
        screen::set_pen(COLOR_WHITE);

        for (DisplayBox div : divisions){
            screen::draw_rect(div.X1,div.Y1,div.X2,div.Y2); // Draw main Rectangle

            div.drawHorizontalLines();
            div.drawVerticalLines();
        }

        while (screen_touch_status_s().touch_status != E_TOUCH_HELD){
            delay(10);
        }
        screen::fill_rect(1,1,240,240); // Draw main Rectangle
        delay(1000000);

    }
}
