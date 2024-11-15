#include "main.h"

using namespace pros;

int teamColor = COLOR_RED;
bool runAutonSelector = true;

screen_touch_status_s_t status;

int row, collum = 0;
int autonSelected = 0;

void autonSelector (){

    screen::erase();
    screen::set_pen(COLOR_WHITE);

    delay(50);

    while (runAutonSelector == true){

        status = screen::touch_status();

        if (teamColor = COLOR_RED){
            screen::set_pen(teamColor);
            screen::fill_rect(0,0,120,120); // Red top button
            screen::set_pen(COLOR_WHITE);
            screen::fill_rect(0,120,120,240); // Redraw bottom button
            screen::draw_rect(0,120,120,240);
        }
        else { // Blue
            screen::set_pen(teamColor);
            screen::draw_rect(0,120,120,240); // Blue bottom button
            screen::set_pen(COLOR_WHITE);
            screen::draw_rect(0,0,120,120); // Redraw top button
            screen::draw_rect(0,0,120,120);
        }

        screen::set_pen(COLOR_WHITE);
        screen::draw_rect(120,0,480,240); // Draws main auton box
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

            if(status.x > 280){ collum = 3; }
            else if(status.x > 200){ collum = 2; }
            else { collum = 3; }

            screen::set_pen(COLOR_GREEN);

            if      (row == 1 && collum == 1){ autonSelected = 1; screen::fill_rect(120,0  ,240,80 ); }
            else if (row == 1 && collum == 2){ autonSelected = 2; screen::fill_rect(240,0  ,360,80 ); }
            else if (row == 1 && collum == 3){ autonSelected = 3; screen::fill_rect(360,0  ,480,80 ); }
            else if (row == 2 && collum == 1){ autonSelected = 4; screen::fill_rect(120,80 ,240,160); }
            else if (row == 2 && collum == 2){ autonSelected = 5; screen::fill_rect(240,80 ,360,160); }
            else if (row == 2 && collum == 3){ autonSelected = 6; screen::fill_rect(360,80 ,480,160); }
            else if (row == 3 && collum == 1){ autonSelected = 7; screen::fill_rect(120,160,240,240); }
            else if (row == 3 && collum == 2){ autonSelected = 8; screen::fill_rect(240,160,360,240); }
            else if (row == 3 && collum == 3){ autonSelected = 9; screen::fill_rect(360,160,480,240); }
            else                             { autonSelected = 0; }


            screen::print(TEXT_MEDIUM,130,10,"Close AWP");



        }

        delay(20);
    }

}