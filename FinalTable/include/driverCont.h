#ifndef DRIVERCONT_H // Prevents file from getting included multiple times
#define DRIVERCONT_H

extern void runDriveCont ();
extern void runColorCalibration();
extern void testColorCalibration();
extern void runComboSystem();
extern void runWallMech();
extern void transit(int forcedStateInput);
extern int comboState;
extern bool instantLift;
extern bool instantLift2;
extern float WMKp;
extern float WMKd;
extern float WMScoringTarget;
extern bool colorSorting;
extern bool specialIntake;
extern bool backClawBool;
extern void dejam(int incomingState);
//extern bool state21stop;
extern float WMForwardTarget;

#endif