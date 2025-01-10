#ifndef ODOMETRY_H // Prevents file from getting included multiple times
#define ODOMETRY_H

extern float xPos;
extern float yPos;
extern float tPos;
extern void odometry(void);
extern void calculateOffsets(void);

#endif