#include "main.h"
bool RGIntake1 = false;
bool RGOuttake1 = false;
bool RGOuttake2 = false;
bool speedmax1 = false;
bool speedmax2 = false;
bool stopintake1 = false;
bool stopintake2 = false;
bool startintake1 = false;
bool speedmax3 = false;
bool startintake2 = false;
bool speedmax4 = false;
bool speedmax5 = false;

bool STARTTHETIMER = false;

void redgoal(void){
	intake.move_voltage(13000.0);
    stopintake1 = true;
    inputvov = 1;
	toPoint(25.0, 22.0); //intake bottom ring
	//intake.move_velocity(0.0);
    /*
    inputvovfp = 1;
    rotkp = 150.0; //200 default
    rotki = 1.0; //0 default
    rotkd = 1250.0; //1625 default
    */
    inputvovfp = 1;
    rotkp = 500.0; //150.0; //160.0; //150
    rotki = 81.6922; //!suggested next tune: 77.25221312
    rotkd = 3350.0; //375.0; //1600
	facepoint(2.0 * xPos - 24.0, 2.0 * yPos - 48.0); //back to goal
    inputvovfp = 0;
    rotkp = 200.0;
    rotki = 0.0;
    rotkd = 1625.0;
	toplinmax = 200.0; //200
    //pauseloop = true;
    toplinkp = 1.25;
    toplinkd = 0.0;
	toPoint(24.0, 42.0);//(24.0, 43.5); //drive to goal
    toplinkp = 2.0;
    toplinkd = 1.0;
	toplinmax = 600.0;
	backClaw.set_value(1);
    delay(500); //let back claw grab the dang goal
    inputvov = 1;
    startintake2 = true;
	toPoint(12.0, -6.0); //drive to place first goal
    intake.move_voltage(13000.0);
    //!delay(1000); //dropping onto first goal
    //intake.move_velocity(0.0);
    //toPoint(24.0, 0.0); //first topoint
    facepoint(xPos + 12.0, yPos + 12.0); //point to 45deg
    backClaw.set_value(0);
    intake.move_voltage(13000.0); //!is this overclocking
    //delay(500); //let the goal fall so you dont drag it?
    //facepoint(36.0, -16.0); //face ring
    stopintake2 = true;
    toPoint(31.5, -10.5); //intake preload
    delay(500);
    //speedmax2 = true;
    speedmax3 = true;
    //toplinmax = 200.0;
    toPoint(0.0, 24.0);//(5.48, 17.95); //go to second goal
    toplinmax = 600.0;
    //delay(250);
    backClaw.set_value(1);
    startintake1 = true;
    facepoint(-24.0, 19.0);
    intake.move_voltage(13000.0);
    delay(500); //dropping onto second goal
    //intake.move_velocity(0.0);
    ploinker.set_value(1);
    rightDrive.move_velocity(150.0);
    leftDrive.move_velocity(150.0);
    delay(1000);
    //backClaw.set_value(0);
}

float goaldist = 5.0; //3.0;

void rednone(void){
    inputvov = 1;
    //pauseloop = 1;
    //toplinmax = 200.0;
    speedmax4 = true;
    //toPointthe2nd(-3.0, 17.0);//(0.0, 24.0); //toward first goal
    toPointthe2nd(0.0 - goaldist * cos(arctan2(0.0 - xPos, 24.0 - yPos)), 24.0 - goaldist * sin(arctan2(0.0 - xPos, 24.0 - yPos))); //go to first goal
    toplinmax = 600.0;
    backClaw.set_value(1);
    delay(175);
    intake.move_voltage(13000.0);
    delay(500);
    //*intake.move_velocity(0.0);
    //toplinmax = 600.0;
    deadzonerad = 6.0;
    //pauseloop = 1;
    toPointthe2nd(-22.0, 22.0); //(-24.0, 24.0); //intake first bottom ring
    intake.move_voltage(13000.0);
    delay(500); //dropping first nonpreload //!add to blue side
    toPointthe2nd(-19.0, 12.0); //back up to avoid blue ring
    //pauseloop = 1;
    toPointthe2nd(-17.0, 36.0); //(-19.0, 38.0); //get second bottom ring
    //delay(500);
    //toPointthe2nd(xPos + 10.0 * cos(currentTheta), yPos + 10.0 * sin(currentTheta)); //back a tad //!if the +/- works dont mess with it
    facepoint(-27.5, 44.5); //face last ring
    //toPointthe2nd(-28.0, 40.0); //get second second bottom ring
    toPointthe2nd(-27.5 - 8.5 * cos(arctan2(-27.5 - xPos, 44.5 - yPos)), 44.5 - 8.5 * sin(arctan2(-27.5 - xPos, 44.5 - yPos))); //!edit blue side get second second bottom ring 
    toPoint(0.0, 24.0); //align for touching goal //!edit blue side
    delay(375);
    facepoint(-6.0, 48.0); //face goal //!edit blue side
    ploinker.set_value(1);
    rightDrive.move_velocity(100.0);
    leftDrive.move_velocity(100.0);
}

void bluegoal(void){ //?copied and pasted redgoal() and converted points with (x,y) -> (-x,y)
	intake.move_voltage(13000.0);
    stopintake1 = true;
    inputvov = 1;
	toPoint(-25.0, 22.0); //intake bottom ring
	//intake.move_velocity(0.0);
    /*
    inputvovfp = 1;
    rotkp = 150.0; //200 default
    rotki = 1.0; //0 default
    rotkd = 1250.0; //1625 default
    */
    inputvovfp = 1;
    rotkp = 500.0; //150.0; //160.0; //150
    rotki = 81.6922; //!suggested next tune: 77.25221312
    rotkd = 3350.0; //375.0; //1600
	facepoint(2.0 * xPos + 24.0, 2.0 * yPos - 48.0); //back to goal
    inputvovfp = 0;
    rotkp = 200.0;
    rotki = 0.0;
    rotkd = 1625.0;
	toplinmax = 200.0; //200
    //pauseloop = true;
    toplinkp = 1.25;
    toplinkd = 0.0;
	toPoint(-24.0, 42.0);//(24.0, 43.5); //drive to goal
    toplinkp = 2.0;
    toplinkd = 1.0;
	toplinmax = 600.0;
	backClaw.set_value(1);
    delay(500); //let back claw grab the dang goal
    inputvov = 1;
    startintake2 = true;
	toPoint(0.0, -6.0); //(-12.0, -6.0); //drive to place first goal //!intentionally different from redgoal
    intake.move_voltage(13000.0);
    //!delay(1000); //dropping onto first goal
    //intake.move_velocity(0.0);
    //toPoint(24.0, 0.0); //first topoint
    facepoint(xPos - 12.0, yPos + 12.0); //point to 45deg
    backClaw.set_value(0);
    intake.move_voltage(13000.0);
    //delay(500); //let the goal fall so you dont drag it?
    //facepoint(36.0, -16.0); //face ring
    stopintake2 = true;
    toPoint(-31.5, -10.5); //intake preload
    delay(500);
    //speedmax2 = true;
    speedmax3 = true;
    //toplinmax = 200.0;
    toPoint(0.0, 24.0);//(5.48, 17.95); //go to second goal
    toplinmax = 600.0;
    //delay(250);
    backClaw.set_value(1);
    startintake1 = true;
    facepoint(24.0, 29.0); //!this is different than redgoal on purpose
    intake.move_voltage(13000.0);
    delay(500); //dropping onto second goal
    //intake.move_velocity(0.0);
    ploinker.set_value(1);
    rightDrive.move_velocity(150.0);
    leftDrive.move_velocity(150.0);
    delay(1000);
    //backClaw.set_value(0);
}

void bluenone(void){ //? (x,y) -> (-x,y)
    inputvov = 1;
    //pauseloop = 1;
    //toplinmax = 200.0;
    speedmax4 = true;
    //toPointthe2nd(-3.0, 17.0);//(0.0, 24.0); //toward first goal
    toPointthe2nd(0.0 - goaldist * cos(arctan2(0.0 - xPos, 24.0 - yPos)), 24.0 - goaldist * sin(arctan2(0.0 - xPos, 24.0 - yPos))); //go to first goal
    toplinmax = 600.0;
    backClaw.set_value(1);
    delay(175);
    intake.move_voltage(13000.0);
    delay(500);
    //*intake.move_velocity(0.0);
    //toplinmax = 600.0;
    deadzonerad = 6.0;
    //pauseloop = 1;
    toPointthe2nd(22.0, 22.0); //(-24.0, 24.0); //intake first bottom ring
    intake.move_voltage(13000.0);
    delay(500); //dropping first nonpreload 
    toPointthe2nd(19.0, 12.0); //(19.0, 12.0); //!intentional //back up to avoid blue ring
    facepoint(16.0, 18.0); //prep for first far ring //!intentional
    //pauseloop = 1;
    toPointthe2nd(17.0, 36.0); //(-19.0, 38.0); //get second bottom ring
    //delay(500); //!just added this
    //toPointthe2nd(xPos + 10.0 * cos(currentTheta), yPos + 10.0 * sin(currentTheta)); //back a tad //!if the +/- works dont mess with it
    facepoint(27.5, 44.5); //face last ring
    //toPointthe2nd(-28.0, 40.0); //get second second bottom ring
    toPointthe2nd(27.5 - 8.5 * cos(arctan2(27.5 - xPos, 42.0 - yPos)), 42.0 - 8.5 * sin(arctan2(27.5 - xPos, 42.0 - yPos))); //!y used to be 44.5 //get second second bottom ring 
    toPoint(0.0, 24.0); //align for touching goal 
    delay(375);
    facepoint(-4.0, 48.0); //face goal //!intentionally not 6.0
    ploinker.set_value(1);
    rightDrive.move_velocity(100.0);
    leftDrive.move_velocity(100.0);
}



void doTask(void){
    while(1){
        if (RGIntake1){
            delay(500);
            intake.move_velocity(600);
            RGIntake1 = false;
        }
        if (RGOuttake1){
            delay(1000);
            intake.move_velocity(-600.0);
            RGOuttake1 = false;
        }
        if (RGOuttake2){
            delay(1000);
            intake.move_velocity(-600.0);
            RGOuttake2 = false;
        }
        if (speedmax1){
            delay(500);
            toplinmax = 200.0;
            speedmax1 = false;
        }
        if (stopintake1){
            delay(1250);
            intake.move_velocity(0.0);
            stopintake1 = false;
        }
        if (stopintake2){
            delay(1375);
            intake.move_velocity(0.0);
            stopintake2 = false;
        }
        if (startintake1){
            delay(750);
            intake.move_voltage(13000.0);
            startintake1 = false;
        }
        if (startintake2){
            delay(500);
            intake.move_voltage(13000.0);
            startintake2 = false;
        }
        if (speedmax4){
            delay(400);
            toplinmax = 100.0;
            speedmax4 = false;
        }
        if (speedmax5){
            delay(750);
            toplinmax = 175.0;
            speedmax5 = false;
        }
        delay(10);
    }
}

void speedlimitanddrop(void){
    //int looped = 0;
    float speedmaxdelay = 500.0;
    while(1){
        if (speedmax3){
            delay(speedmaxdelay); //!250; pushed later to get to end faster and drop goal earlier
            toplinmax = 175.0;
            speedmax3 = false;
            //looped += speedmaxdelay;
        }
        /*
        if (looped >= 14500){
            backClaw.set_value(0);
        }
        if (STARTTHETIMER == true){
            delay(50);
            looped += 50;
        }
        */
        else{delay(10);}
    }
}

/* //?last year auton stuff -- to use as template only
	if (autonSelector == 1){close();}
	else if (autonSelector == 2){closeawp();}
	else if (autonSelector == 3){closedoal();}
	else if (autonSelector == 4){closerush();}
	else if (autonSelector == 5){farawp();}
	else if (autonSelector == 6){farsix();}
	else if (autonSelector == 7){farshortawp();}
	else if (autonSelector == 8){screen::print(TEXT_MEDIUM,180,120,"COOOKED");}
	else{autonSelector = 999;} // you're cooked
*/

void skillsgoat(void){
    //toplinmax = 100.0;
    goaldist = 6.0;
    toPoint(-12.0 - goaldist * cos(arctan2(-12.0 - xPos, 0.0 - yPos)), 0.0 - goaldist * sin(arctan2(-12.0 - xPos, 0.0 - yPos)));
    toplinmax = 600.0;
    backClaw.set_value(1);
    delay(500);
    intake.move_voltage(13000.0);
    delay(1500);
    toPoint(-48.0, -12.0); //intake first ring
    delay(500);
    toPoint(-48.0, 0.0); //intake second ring
    delay(500);
    toPoint(-60.0, 0.0); //intake third ring
    delay(500);
    toPoint(-48.0, 24.0); //intake fourth ring
    delay(500);
    toPoint(-24.0, 24.0); //intake fifth and final first goal ring
    delay(500);
    facepoint(2.0 * xPos + 66.0, 2.0 * yPos + 18.0);
    delay(500);
    goaldist = 12.0;
    toPoint(-66.0 - goaldist * cos(arctan2(-66.0 - xPos, -18.0 - yPos)), -18.0 - goaldist * sin(arctan2(-66.0 - xPos, -18.0 - yPos))); //score first goal
    backClaw.set_value(0);
    delay(500);
    toPoint(-48.0, 0.0); //leave scoring first goal
    delay(500);
    facepoint(2.0 * xPos - 24.0, 2.0 * yPos - 0.0); //align for grabbing second goal
    delay(500);
    //speedmax5 = true;
    goaldist = 8.0;
    toPoint(24.0 - goaldist * cos(arctan2(-24.0 - xPos, 0.0 - yPos)), 0.0 - goaldist * sin(arctan2(24.0 - xPos, 0.0 - yPos))); //go to second goal
    backClaw.set_value(1);
    rightDrive.move_velocity(-100.0);
    leftDrive.move_velocity(-100.0);
    delay(1000);
    rightDrive.brake();
    leftDrive.brake();
    toplinmax = 600.0;
    toPoint(24.0, 24.0); //intake first ring
    delay(500);
    facepoint(48.0, 24.0);
    toPoint(48.0, 24.0); //intake second ring
    delay(500);
    toPoint(60.0, 48.0); //intake third ring
    delay(500);
    facepoint(48.0, -12.0); //look for fourth ring
    delay(500);
    toPoint(48.0, -12.0); //intake fourth and fifth rings together?
    delay(500);
    facepoint(60.0, 0.0); //face last ring
    delay(500);
    toPoint(60.0, 0.0); //intake sixth ring
    facepoint(2.0 * xPos - 66.0, 2.0 * yPos + 18.0);
    delay(500);
    toPoint(66.0 - goaldist * cos(arctan2(66.0 - xPos, -18.0 - yPos)), -18.0 - goaldist * sin(arctan2(66.0 - xPos, -18.0 - yPos))); //score second goal
    backClaw.set_value(0);
    delay(500);
    toPoint(24.0, 72.0); //intake first ring for goal three
    delay(250);
    intake.move_velocity(0.0);
    facepoint(2.0 * xPos - 0.0, 20 * yPos + 96.0); //back to third goal
    toPoint(0.0 - goaldist * cos(arctan2(0.0 - xPos, 96.0 - yPos)), 96.0 - goaldist * sin(arctan2(0.0 - xPos, 96.0 - yPos))); //grab third goal
    backClaw.set_value(1);
    delay(500);
    intake.move_voltage(13000.0);
    toPoint(-24.0, 72.0);
    delay(500);
    toPoint(-48.0, 72.0);
    delay(500);
    toPoint(-60.0, 48.0);
}

void testing(void){
    //rotarc(-48.0, 24.0, -pi);
    pgrarc(24.0, 24.0, 42.0, 1, 1, 1);
    lcd::set_text(0, "finished");
}