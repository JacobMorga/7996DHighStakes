#include "main.h"

using namespace pros;

int teamColor = COLOR_RED; //for skills (we would never want default blue anyway)

bool runAutonSelector = true;

screen_touch_status_s_t status;

int row, column = 0;
int autonSelected = 0;
int xTxtOffset = 30;
int yTxtOffset = 30;

void autonSelector (){

    screen::erase();

    delay(50);

    while (runAutonSelector == true){

        status = screen::touch_status();

                       if (teamColor == COLOR_RED){
		std::cout << "RED";
	}
	else if (teamColor == COLOR_BLUE){
		std::cout << "BLUE";
	}
	else{
		std::cout << "NO COLOR";
	}

	std::cout << " : " << autonSelected << "\n";

        screen::set_pen(COLOR_WHITE);
        screen::set_eraser(COLOR_BLACK);

        if (teamColor == COLOR_RED){
            screen::erase_rect(0,0,119,240);
            delay(20);
            screen::set_pen(teamColor);
            screen::fill_rect(0,0,119,119);
            screen::set_pen(COLOR_WHITE);
            screen::draw_rect(0,120,119,240);

        }
        else {
            screen::erase_rect(0,0,119,240);
            screen::set_pen(teamColor);
            delay(20);
            screen::fill_rect(0,120,119,240);
            screen::set_pen(COLOR_WHITE);
            screen::draw_rect(0,0,119,119);
        }

        screen::set_pen(COLOR_WHITE);
        screen::erase_rect(120,0,480,240); // Draws main auton box
        screen::draw_line(240,0,240,240); // Splits auton box vertically 
        screen::draw_line(360,0,360,240);
        screen::draw_line(120,80,480,80); // Splits auton box horizontally  
        screen::draw_line(120,160,480,160);
        
        if (status.x < 120){ // Red and blue button

            if (status.y < 120){ teamColor = COLOR_RED; }
            else { teamColor = COLOR_BLUE; }
        }
        else { // Auton squares
            
            if(status.y < 80){ row = 1; }
            else if(status.y < 160){ row = 2; }
            else{ row = 3; }

            if(status.x > 360){ column = 3; }
            else if(status.x > 240){ column = 2; }
            else { column = 1; }

            std::cout << row << ":" << column << "\n";

            screen::set_pen(COLOR_GREEN);

            if      (row == 1 && column == 1){ autonSelected = 1; screen::fill_rect(120,0  ,240,80 ); }
            else if (row == 1 && column == 2){ autonSelected = 2; screen::fill_rect(240,0  ,360,80 ); }
            else if (row == 1 && column == 3){ autonSelected = 3; screen::fill_rect(360,0  ,480,80 ); }
            else if (row == 2 && column == 1){ autonSelected = 4; screen::fill_rect(120,80 ,240,160); }
            else if (row == 2 && column == 2){ autonSelected = 5; screen::fill_rect(240,80 ,360,160); }
            else if (row == 2 && column == 3){ autonSelected = 6; screen::fill_rect(360,80 ,480,160); }
            else if (row == 3 && column == 1){ autonSelected = 7; screen::fill_rect(120,160,240,240); }
            else if (row == 3 && column == 2){ autonSelected = 8; screen::fill_rect(240,160,360,240); }
            else if (row == 3 && column == 3){ autonSelected = 9; screen::fill_rect(360,160,480,240); }
            else                             { autonSelected = 0; }

        }
        
        screen::set_pen(COLOR_WHITE);
        screen::print(TEXT_MEDIUM_CENTER, 120 + xTxtOffset, 0   + yTxtOffset, "BPos AWP");
        screen::print(TEXT_MEDIUM_CENTER, 240 + xTxtOffset, 0   + yTxtOffset, "BNeg AWP");
        screen::print(TEXT_MEDIUM_CENTER, 360 + xTxtOffset, 0   + yTxtOffset, "RPos AWP");
        screen::print(TEXT_MEDIUM_CENTER, 120 + xTxtOffset, 80  + yTxtOffset, "RNeg AWP");
        screen::print(TEXT_MEDIUM_CENTER, 240 + xTxtOffset, 80  + yTxtOffset, "BPos Max");
        screen::print(TEXT_MEDIUM_CENTER, 360 + xTxtOffset, 80  + yTxtOffset, "BNeg Max");
        screen::print(TEXT_MEDIUM_CENTER, 120 + xTxtOffset, 160 + yTxtOffset, "RPos Max");
        screen::print(TEXT_MEDIUM_CENTER, 240 + xTxtOffset, 160 + yTxtOffset, "RNeg Max");
        screen::print(TEXT_MEDIUM_CENTER, 360 + xTxtOffset, 160 + yTxtOffset, "Skills");

                               if (teamColor == COLOR_RED){
		std::cout << "RED";
	}
	else if (teamColor == COLOR_BLUE){
		std::cout << "BLUE";
	}
	else{
		std::cout << "NO COLOR";
	}
    	std::cout << " : " << autonSelected << "\n";

        while(status.touch_status == E_TOUCH_HELD){
            status = pros::screen::touch_status();
            delay(20);
        }
        while(status.touch_status != E_TOUCH_HELD){
            status = pros::screen::touch_status();
            delay(20);
        }

        if (status.x < 120){ // Red and blue button

            if (status.y < 120){ teamColor = COLOR_RED; }
            else { teamColor = COLOR_BLUE; }
        }

        screen::erase();

        delay(20);

        
    }

}