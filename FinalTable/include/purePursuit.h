#ifndef PUREPURSUIT_H // Prevents file from getting included multiple times
#define PUREPURSUIT_H
using namespace std;

extern void purePursuit (vector<coord> path, float lookAheadDisPP, float speedCap, bool ppSmooth);
extern void bezierCurve(coord p1, coord p2, coord p3, coord p4, coord p5, int n, vector<coord>& listInput);
extern vector<string> graphingPoints;

extern int whatToRun;
extern void runCodeDuringCode ();

#endif