#ifndef AUTON_H
#define AUTON_H
extern void bluePositive(void);
extern void blueNegative(void);
extern void redPositive(void);
extern void redNegative(void);
extern void skills(void);
extern float goalDist;
extern float cornerDist;
extern float ringStackDist;
extern float clearingDist;
extern float wallStakeDist;
extern bool redAWP;
extern bool blueAWP;
extern void defaultAutoRedPos();
extern void defaultAutoRedNeg();
extern void defaultAutoBluePos();
extern void defaultAutoBlueNeg();
extern void simpleRedRush();
extern void simpleBlueRush();
extern void blueHooker();
#endif