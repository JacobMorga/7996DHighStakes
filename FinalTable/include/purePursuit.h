#ifndef PUREPURSUIT_H // Prevents file from getting included multiple times
#define PUREPURSUIT_H

extern void doThePurePursuit (coord followPoint, vector<coord> path);
extern std::vector<coord> bezierCurve (coord p1, coord p2, coord p3, coord p4, coord p5, int n);
extern void graphThePath (vector<coord> targetPath, vector<coord> actualPath);

#endif