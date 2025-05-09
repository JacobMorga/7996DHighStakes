#ifndef AUTON_H
#define AUTON_H
extern void bluePosAWP();
extern void blueNegAWP();
extern void redPosAWP();
extern void redNegAWP();
extern void bluePosMax();
extern void blueNegMax();
extern void redPosMax();
extern void redNegMax();
extern void skills1();
extern void skills2();
extern void autonTesting();
extern float goalDist;
extern float cornerDist;
extern float ringStackDist;
extern float clearingDist;
extern float wallStakeDist;
extern bool runThisCodeShort;
extern void bluePos();
extern void blueNeg();
extern void blueSolo();
extern void redPos();
extern void redNeg();
extern void redSolo();
extern void blueNegAS();
extern void redNegAS();
extern bool ladderTouch;
extern bool skipAS;
extern void redPosNoRush();
extern void bluePosNoRush();
#endif