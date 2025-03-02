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
extern float WMKp;
extern float WMKd;
extern float WMScoringTarget;
extern bool colorSorting;

#endif