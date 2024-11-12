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


/*
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

pros::screen_touch_status_s_t status;

void autonSelectorNEW (vector<DisplayBox> graph){
    while(1){

        delay(100);
        screen::erase();
        delay(5); //? idk if i need this
        screen::set_pen(COLOR_WHITE);
        screen::set_eraser(COLOR_BLACK);

        status = pros::screen::touch_status();
        
        for (DisplayBox div : graph){
            screen::draw_rect(div.X1,div.Y1,div.X2,div.Y2); // Draw boxes

            if (status.touch_status == TOUCH_HELD){
                div.handleButtons();
            }

            div.drawHorizontalLines();
            div.drawVerticalLines();
        }


        screen::fill_rect(1,1,240,240); // Draw main Rectangle
        delay(1000000);
    }


}
*/
int autonSelector = 0;

void AUTONSELECTORFUNC (void){

	pros::screen_touch_status_s_t status;

    autonSelector = 0;

    screen::set_pen(COLOR_WHITE);
    screen::set_eraser(COLOR_BLACK);
	screen::erase();

    delay(20);
	
    screen::draw_line(240,0,240,240);
	screen::draw_line(120,0,120,240);
	screen::draw_line(360,0,360,240);
	screen::draw_line(0,120,480,120);

    screen::print(TEXT_MEDIUM,10,10,"Goal Red");
	screen::print(TEXT_MEDIUM,130,10,"Goal Blue");
	screen::print(TEXT_MEDIUM,250,10,"No Goal Red");
	screen::print(TEXT_MEDIUM,370,10,"No Goal Blue");
    screen::print(TEXT_MEDIUM,10,130,"Skills");
	screen::print(TEXT_MEDIUM,130,130,"Skills");
	screen::print(TEXT_MEDIUM,250,130,"Skills");
	screen::print(TEXT_MEDIUM,370,130,"Testing");
    
    status = pros::screen::touch_status();

    while(status.touch_status != E_TOUCH_HELD){
        status = pros::screen::touch_status();
        delay(20);
    }
	screen::erase();
	delay(20);
	screen::set_pen(COLOR_GREEN);


    if(status.y <= 120){

		if (status.x <= 120){
			screen::draw_rect(0,0,120,120);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,30,30,"Goal Red");
			autonSelector = 1;
		}
		else if (status.x <= 240){
			screen::draw_rect(120,0,240,120);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,130,10,"Goal Blue");
			autonSelector = 2;
		}
		else if (status.x <= 360){
			screen::draw_rect(240,0,360,120);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,250,10,"No Goal Red");
			autonSelector = 3;
		}
		else if (status.x <= 480){
			screen::draw_rect(360,0,480,120);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,370,10,"No Goal Blue");
			autonSelector = 4;
		}
		else{
			screen::draw_rect(120,60,360,180);
			screen::set_pen(COLOR_RED);
			screen::print(TEXT_MEDIUM,180,120,"COOOKED");
			autonSelector = 999;
		}
    }
	else{

        
		if (status.x <= 120){
			screen::draw_rect(0,120,120,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,10,130,"Skills");
			autonSelector = 5;
		}
		else if (status.x <= 240){
			screen::draw_rect(120,120,240,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,130,130,"Skills");
			autonSelector = 5;
		}
		else if (status.x <= 360){
			screen::draw_rect(240,120,360,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,250,130,"Skills");
			autonSelector = 5;
		}
		else if (status.x <= 480){
			screen::draw_rect(360,120,480,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,370,130,"Testing");
			autonSelector = 6;
		}
		else{
			screen::draw_rect(120,60,360,180);
			screen::set_pen(COLOR_RED);
			screen::print(TEXT_MEDIUM,180,120,"COOOKED");
			autonSelector = 999;
		}
        
    }

	delay(100);



}
