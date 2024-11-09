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

void rednone(void){
    inputvov = 1;

    //pauseloop = 1;
    //toplinmax = 200.0;
    speedmax4 = true;
    toPointthe2nd(-3.0, 17.0);//(0.0, 24.0); //toward first goal
    //toPointthe2nd(0.0 -);
    toplinmax = 600.0;
    backClaw.set_value(1);
    delay(500);
    intake.move_voltage(13000.0);
    delay(1000);
    //*intake.move_velocity(0.0);
    //toplinmax = 600.0;
    deadzonerad = 6.0;
    //pauseloop = 1;
    toPointthe2nd(-22.0, 22.0); //(-24.0, 24.0); //intake first bottom ring
    intake.move_voltage(13000.0);
    toPointthe2nd(-18.0, 18.0);
    //pauseloop = 1;
    toPointthe2nd(-26.75, 38.0); //get second bottom ring
    delay(500);
    toPointthe2nd(-26.75, 24.0); //back a tad
    toPointthe2nd(-21.25, 38.0); //get second second bottom ring
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

void bluenone(void){

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
            delay(325);
            toplinmax = 100.0;
            speedmax4 = false;
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