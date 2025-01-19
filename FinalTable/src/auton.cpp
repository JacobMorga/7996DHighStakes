#include "main.h"

using namespace pros;
float goaldist = 5.0; //3.0;

void bluePositive(){ //currently copied and pasted from redpositive then all x coordinates' signs switched
    teamColor = COLOR_BLUE;
	intakeState = 5; //5 to stop, 8 to go to 3V //!CHANGE THIS JOHN TO 5
    rotKP = 350.0;
    linKP = 25.0;
	toPoint(-27.0, 22.0, 0, 0); //(25.0, 22.0); //intake bottom ring
    linKP = 15.0;
    toPoint(xPos - 8.0, yPos, 1, 1); // get ready to grab goal
    rotKP = 220.0;
    delay(250);
	facePoint(2.0 * xPos + 24.0, 2.0 * yPos - 48.0); //back to goal
	toPoint(-24.5, 43.0, 1, 0);//(24.0, 43.5); //drive to goal
	backClaw.set_value(1);
    delay(500); //let back claw grab the dang goal
    intakeState = 6;
    rotKP = 350.0; //50
	toPoint(0.0, -6.0, 1, 1); //drive to place first goal
    rotKP = 220.0;
    backClaw.set_value(0);
    facePoint(xPos - 12.0, yPos); //point left
    intakeState = 5; //! THIS ONE TOO
    toPoint(-28.0, -8.5, 0, 0); //intake preload
    delay(500);
    linKP = 10.0; //usually 15.0
    toPoint(4.0, 24.0, 1, 0);//(0.0, 24.0); (5.48, 17.95); //go to second goal
    linKP = 15.0;
    backClaw.set_value(1);
    delay(100); //to grab the thing
    intakeState = 2;
    //intakeState = 3;
    facePoint(24.0, 24.0);
    delay(500); //dropping onto second goal
    rightClearer.set_value(1);
    rightDrive.move_velocity(150.0);
    leftDrive.move_velocity(150.0);
    delay(1000);
}

void blueNegative(){ //currently copied and pasted from rednegative with x signs inverted
    teamColor = COLOR_BLUE;
    rotKP = 220.0;
    linKP = 10.0;
    toPoint(0.0, 24.0, 1, 0);
    //toPoint(0.0 - goaldist * cos(arctan2(0.0 - xPos, 24.0 - yPos)), 24.0 - goaldist * sin(arctan2(0.0 - xPos, 24.0 - yPos)), 1, 0); //go to first goal
    linKP = 15.0;
    delay(500);
    backClaw.set_value(1);
    intakeState = 3;
    rotKP = 350.0;
    toPoint(30.0, 30.0, 0, 1); //(-24.0, 24.0); //intake first bottom ring    
    toPoint(26.0, 41.0, 0, 1); //intake first auton line ring
    toPoint(xPos, yPos - 18.0, 1, 1); //back up a sec
    toPoint(20.0, 39.0, 0, 1); //intake second auton line ring
    toPoint(0.0, 24.0, 0, 1); //get away from line
    toPoint(0.0, 48.0, 0, 0); //hit ladder
}

void redPositive(){
    teamColor = COLOR_RED;
	intakeState = 5;
    rotKP = 350.0;
	toPoint(27.0, 22.0, 0, 0); //(25.0, 22.0); //intake bottom ring
    toPoint(xPos + 8.0, yPos, 1, 1); // get ready to grab goal
    rotKP = 220.0;
    while (intakeState == 5){delay(10.0);}delay(350);
	facePoint(2.0 * xPos - 24.0, 2.0 * yPos - 48.0); //back to goal
	toPoint(24.5, 41.5, 1, 0);//(24.0, 43.5); //drive to goal
	backClaw.set_value(1);
    delay(500); //let back claw grab the dang goal
    intakeState = 6;
    rotKP = 350.0; //50
	toPoint(12.0, -6.0, 0, 1); //drive to place first goal
    rotKP = 220.0;
    facePoint(xPos + 12.0, yPos + 12.0); //point to 45deg
    backClaw.set_value(0);
    intakeState = 5;
    toPoint(28.0, -10.5, 0, 0); //intake preload
    delay(500);
    linKP = 10.0; //usually 15.0
    toPoint(-2.0, 26.0, 1, 0);//(0.0, 24.0); (5.48, 17.95); //go to second goal
    linKP = 15.0;
    backClaw.set_value(1);
    delay(100); //to grab the thing
    intake.move_voltage(11000.0);
    intakeState = 3;
    facePoint(-24.0, 24.0);
    delay(500); //dropping onto second goal
    rightClearer.set_value(1);
    rightDrive.move_velocity(150.0);
    leftDrive.move_velocity(150.0);
    delay(1000);
}

void redNegative(){
    teamColor = COLOR_RED;
    rotKP = 220.0;
    toPoint(0.0 - goaldist * cos(arctan2(0.0 - xPos, 24.0 - yPos)), 24.0 - goaldist * sin(arctan2(0.0 - xPos, 24.0 - yPos)), 1, 0); //go to first goal
    backClaw.set_value(1);
    intakeState = 3;
    rotKP = 350.0;
    toPoint(-30.0, 30.0, 0, 1); //(-24.0, 24.0); //intake first bottom ring    
    toPoint(-26.0, 44.0, 0, 1); //intake first auton line ring
    toPoint(xPos, yPos - 18.0, 1, 1); //back up a sec
    toPoint(-20.0, 42.0, 0, 1); //intake second auton line ring
    toPoint(0.0, 24.0, 0, 1); //get away from line
    toPoint(0.0, 48.0, 0, 0); //hit ladder
}

void skills(){
    teamColor = COLOR_RED;
    rotKP = 350.0;
    linKP = 20.0; //!delete this if it nacks
    toPoint(-24.0, 0.0, 1, 0); //get first goal
    backClaw.set_value(1);
    linKP = 25.0;
    intakeState = 2;
    rotKP = 600.0;
    toPoint(-48.0, -12.0, 0, 1); //intake first ring
    toPoint(-60.0, 0.0, 0, 1); //intake second ring
    toPoint(-36.0, 0.0, 0, 1); //intake third ring
    toPoint(-48.0, 24.0, 0, 1); //intake fourth ring
    rotKP = 1000.0;
    toPoint(-18.0, 24.0, 0, 1); //intake fifth and final first goal ring
    rotKP = 600.0;
    delay(500); //get that last pesky ring
    goaldist = 12.0;
    distLimit = 12.0;
    toPoint(-66.0 - goaldist * cos(arctan2(-66.0 - xPos, -18.0 - yPos)), -18.0 - goaldist * sin(arctan2(-66.0 - xPos, -18.0 - yPos)), 1, 0); //score first goal
    delay(2000); //put all the rings on the goal before you leave 

    //! ADDED ON LEAGUE DAY
    intakeState = 1;
    delay(150);
    intakeState = 2;
    //! ADDED ON LEAGUE DAY

    distLimit = 5.0;
    backClaw.set_value(0);
    toPoint(xPos, yPos + 12.0, 0, 1); // leave scoring first goal (remember the time it took it out with it 💀)
    linKP = 10.0;
    toPoint(24.0, 0.0, 1, 0); //second goal
    linKP = 25.0;
    backClaw.set_value(1);
    toPoint(24.0, 30.0, 0, 1); //intake first ring
    toPoint(48.0, 24.0, 0, 1); //intake second ring
    toPoint(58.0, 54.0, 0, 1); //intake third ring
    toPoint(44.0, 0.0, 0, 1); //intake fourth ring
    toPoint(44.0, -16.0, 0, 0); //intake fifth ring //! smooth 0 on purpose
    toPoint(61.0, -18.0, 0, 1); //clear out the corner perchance?
    toPoint(64.0, 3.0, 0, 1); //intake sixth ring
    goaldist = 6.0;
    toPoint(66.0 - goaldist * cos(arctan2(66.0 - xPos, -18.0 - yPos)), -18.0 - goaldist * sin(arctan2(66.0 - xPos, -18.0 - yPos)), 1, 0); //score second goal
    delay(2000);

    //! ADDED ON LEAGUE DAY
    intakeState = 1;
    delay(150);
    intakeState = 2;
    //! ADDED ON LEAGUE DAY

    backClaw.set_value(0);
    intakeState = 5;
    linKP = 18.0;
    toPoint(48.0, 72.0, 0, 1); //intake first ring for goal three
    delay(250); //let ring stop securely on intake
    linKP = 10.0;
    toPoint(0.0, 96.0, 1, 0); //goal three grabbage
    linKP = 25.0;
    backClaw.set_value(1);
    intakeState = 2;
    rotKP = 1000.0;
    linKP = 32.0;
    toPoint(-24.0, 72.0, 0, 1); //three bare reds
    rotKP = 350.0;
    toPoint(-48.0, 72.0, 0, 1);
    rotKP = 1000.0;
    toPoint(-54.0, 36.0, 0, 1); //edited to dodge pole sabotage
    toPoint(-48.0, 40.0, 0, 1); //away from the wall so you don't hit it spinning
    linKP = 25.0;

    //toPoint(-36.0, 108.0, 0, 1); //prepare for corner clear

    
    toPoint(-46.0, 94.0, 0, 0); //covered red 1 //! 0 on purpose, it needs to handle all the rings slowly
    rotKP = 350.0;
    intakeState = 3; //i know this is supposed to be a line earlier but since smooth is on this is better
    toPoint(-46.0, 106.0, 0, 0); //covered red 2 //!from here we made everything 4 inches to the right (+x) (this one is 2)
    delay(2000); //let colors cycle through
    

    //rightDrive.brake();
    //leftDrive.brake();

    toPoint(-24.0, 72.0, 0, 1);
    distLimit = 12.0;
    toPoint(-66, 102.0, 1, 0); //push red goal into corner
    distLimit = 5.0;
    backClaw.set_value(0);
    toPoint(0.0, 96.0, 0, 1); //avoid first blue goal
    toPoint(24.0, 108.0, 0, 1); //get second blue goal
    toPoint(72.0, 108.0, 0, 0); //push blue into corner
    toPoint(24.0, 96.0, 1, 0); //leave at the end

    /*
    clearer.set_value(1);
    toPoint(-60.0, 108.0, 0, 1); //start corner clearing
    toPoint(-68.0, 72.0, 0, 0); //intake after clearing
    delay(2000);
    toPoint(-68.0, 108.0, 1, 0); //score final corner goal
    backClaw.set_value(0);
    toPoint(-68.0, 72.0, 0, 0); //leave after scoring
    */


    /*
    toPoint(-56.0, 106.0, 0, 0); //remove blue from corner
    toPoint(-40.0, 96.0, 1, 0); //leave corner
    delay(1500); //let it throw that blue away
    toPoint(-56.0, 106.0, 0, 0); //remove blue from corner
    toPoint(-40.0, 96.0, 1, 0); //leave corner
    toPoint(-56, 106.0, 1, 0); //score third goal
    delay(1000);
    backClaw.set_value(0);
    intakeState = 0;
    toPoint(-44.0, 72.0, 0, 0); //leave third goal
    */
}