#include "main.h"
int autonSelectorVar = 0;

void lastYear(void){

	pros::screen_touch_status_s_t status;

    autonSelectorVar = 0;

    screen::set_pen(COLOR_WHITE);
    screen::set_eraser(COLOR_BLACK);
	screen::erase();

    delay(20);
	
    screen::draw_line(240,0,240,240);
	screen::draw_line(120,0,120,240);
	screen::draw_line(360,0,360,240);
	screen::draw_line(0,120,480,120);

    screen::print(TEXT_MEDIUM,10,10,"Close");
	screen::print(TEXT_MEDIUM,130,10,"Close AWP");
	screen::print(TEXT_MEDIUM,250,10,"Close Doal");
	screen::print(TEXT_MEDIUM,370,10,"Close Rush");
    screen::print(TEXT_MEDIUM,10,130,"Far AWP");
	screen::print(TEXT_MEDIUM,130,130,"Far 6");
	screen::print(TEXT_MEDIUM,250,130,"Far Sh AWP");
	screen::print(TEXT_MEDIUM,370,130,"Empty");
    
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
			screen::print(TEXT_MEDIUM,30,30,"Close");
			autonSelectorVar = 1;
		}
		else if (status.x <= 240){
			screen::draw_rect(120,0,240,120);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,130,10,"Close AWP");
			autonSelectorVar = 2;
		}
		else if (status.x <= 360){
			screen::draw_rect(240,0,360,120);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,250,10,"Close Doal");
			autonSelectorVar = 3;
		}
		else if (status.x <= 480){
			screen::draw_rect(360,0,480,120);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,370,10,"Close Rush");
			autonSelectorVar = 4;
		}
		else{
			screen::draw_rect(120,60,360,180);
			screen::set_pen(COLOR_RED);
			screen::print(TEXT_MEDIUM,180,120,"COOOKED");
			autonSelectorVar = 999;
		}
    }
	else{

		if (status.x <= 120){
			screen::draw_rect(0,120,120,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,10,130,"Far AWP");
			autonSelectorVar = 5;
		}
		else if (status.x <= 240){
			screen::draw_rect(120,120,240,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,130,130,"Far 6");
			autonSelectorVar = 6;
		}
		else if (status.x <= 360){
			screen::draw_rect(240,120,360,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,250,130,"Far Sh AWP");
			autonSelectorVar = 7;
		}
		else if (status.x <= 480){
			screen::draw_rect(360,120,480,240);
			screen::set_pen(COLOR_WHITE);
			screen::print(TEXT_MEDIUM,370,130,"Empty");
			autonSelectorVar = 8;
		}
		else{
			screen::draw_rect(120,60,360,180);
			screen::set_pen(COLOR_RED);
			screen::print(TEXT_MEDIUM,180,120,"COOOKED");
			autonSelectorVar = 999;
		}
    }

	delay(100);
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