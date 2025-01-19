#ifndef MOTIONALGS_H // Prevents file from getting included multiple times
#define MOTIONALGS_H

extern void facePoint(float xTar, float yTar);
extern void linear(float distance);
extern void toPoint(float xTar, float yTar, float reversed, bool smooth);
extern void rotArc(float xTar, float yTar, float tTar);
extern float rotKP;
extern float linKP;
extern float distLimit;


#endif