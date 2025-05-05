#include "main.h"

using namespace pros;

int teamColor = COLOR_RED; //for skills (we would never want default blue anyway)

bool runAutonSelector = true;

screen_touch_status_s_t status;

int row, column = 0;
int autonSelected = 0;
int xTxtOffset = 20;
int yTxtOffset = 20;
bool driverSkills = 0;

void autonSelector (){

    driverSkills = 0;

    screen::erase();

    delay(50);

    while (runAutonSelector == true){

        status = screen::touch_status();

            screen::set_pen(COLOR_WHITE);
            screen::set_eraser(COLOR_BLACK);
            screen::erase();

            /*
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
            */

            //screen::set_pen(COLOR_WHITE);
            screen::draw_line(240,0,240,240); // Splits auton box vertically 
            screen::draw_line(120,0,120,240);
            screen::draw_line(360,0,360,240);

            screen::draw_line(0,60,480,60);
            screen::draw_line(0,120,480,120);
            screen::draw_line(0,180,480,180);
                
            if(status.y < 60){ row = 1; }
            else if(60 < status.y && status.y < 120){ row = 2; }
            else if(120 < status.y && status.y < 180){ row = 3; }
            else{ row = 4; }

            if(status.x < 120){ column = 1; }
            else if(120 < status.x && status.x < 240){ column = 2; }
            else if(240 < status.x && status.x < 360){ column = 3; }
            else { column = 4; }

            if(status.y == 0 && status.x == 0){row = 0; column = 0;} // only happens on first loop (default press is 0,0)

            if(row == 4 && column == 3){driverSkills = 1;}

            std::cout << row << ":" << column << "\n";

            screen::set_pen(COLOR_GREEN);

            if      (row == 1 && column == 1){ autonSelected = 1;  screen::fill_rect(0  ,0  ,120,60 ); }
            else if (row == 1 && column == 2){ autonSelected = 2;  screen::fill_rect(120,0  ,240,60 ); }
            else if (row == 1 && column == 3){ autonSelected = 3;  screen::fill_rect(240,0  ,360,60 ); }
            else if (row == 1 && column == 4){ autonSelected = 4;  screen::fill_rect(360,0  ,480,60 ); }
            else if (row == 2 && column == 1){ autonSelected = 5;  screen::fill_rect(0  ,60 ,120,120); }
            else if (row == 2 && column == 2){ autonSelected = 6;  screen::fill_rect(120,60 ,240,120); }
            else if (row == 2 && column == 3){ autonSelected = 7;  screen::fill_rect(240,60 ,360,120); }
            else if (row == 2 && column == 4){ autonSelected = 8;  screen::fill_rect(360,60 ,480,120); }
            else if (row == 3 && column == 1){ autonSelected = 9;  screen::fill_rect(0  ,120,120,180); }
            else if (row == 3 && column == 2){ autonSelected = 10; screen::fill_rect(120,120,240,180); }
            else if (row == 3 && column == 3){ autonSelected = 11; screen::fill_rect(240,120,360,180); }
            else if (row == 3 && column == 4){ autonSelected = 12; screen::fill_rect(360,120,480,180); }
            else if (row == 4 && column == 1){ autonSelected = 13; screen::fill_rect(0  ,180,120,240); }
            else if (row == 4 && column == 2){ autonSelected = 14; screen::fill_rect(120,180,240,240); }
            else if (row == 4 && column == 3){ autonSelected = 15; screen::fill_rect(240,180,360,240); }
            else if (row == 4 && column == 4){ autonSelected = 16; screen::fill_rect(360,180,480,240); }
            else                             { autonSelected = 0; }

            
            screen::set_pen(COLOR_WHITE);

            screen::print(TEXT_MEDIUM_CENTER, 0  +xTxtOffset,0  +yTxtOffset, "Blue Pos"); 
            screen::print(TEXT_MEDIUM_CENTER, 120+xTxtOffset,0  +yTxtOffset, "Blue Neg"); 
            screen::print(TEXT_MEDIUM_CENTER, 240+xTxtOffset,0  +yTxtOffset, "Red Pos"); 
            screen::print(TEXT_MEDIUM_CENTER, 360+xTxtOffset,0  +yTxtOffset, "Red Neg"); 
            screen::print(TEXT_MEDIUM_CENTER, 0  +xTxtOffset,60 +yTxtOffset, "BP M"); 
            screen::print(TEXT_MEDIUM_CENTER, 120+xTxtOffset,60 +yTxtOffset, "BN M"); 
            screen::print(TEXT_MEDIUM_CENTER, 240+xTxtOffset,60 +yTxtOffset, "RP M"); 
            screen::print(TEXT_MEDIUM_CENTER, 360+xTxtOffset,60 +yTxtOffset, "RN M"); 
            screen::print(TEXT_MEDIUM_CENTER, 0  +xTxtOffset,120+yTxtOffset, "BP A S"); 
            screen::print(TEXT_MEDIUM_CENTER, 120+xTxtOffset,120+yTxtOffset, "BN A S"); 
            screen::print(TEXT_MEDIUM_CENTER, 240+xTxtOffset,120+yTxtOffset, "RP A S"); 
            screen::print(TEXT_MEDIUM_CENTER, 360+xTxtOffset,120+yTxtOffset, "RN A S"); 
            screen::print(TEXT_MEDIUM_CENTER, 0  +xTxtOffset,180+yTxtOffset, "Skills"); 
            screen::print(TEXT_MEDIUM_CENTER, 120+xTxtOffset,180+yTxtOffset, "Skills"); 
            screen::print(TEXT_MEDIUM_CENTER, 240+xTxtOffset,180+yTxtOffset, "Driver S"); 
            screen::print(TEXT_MEDIUM_CENTER, 360+xTxtOffset,180+yTxtOffset, "Testing");


            while(status.touch_status == E_TOUCH_HELD){
                status = pros::screen::touch_status();
                delay(20);
            }
            while(status.touch_status != E_TOUCH_HELD){
                status = pros::screen::touch_status();
                delay(20);
            }

        //screen::erase();

        delay(20);
    }
}