using namespace pros;

extern void rotlinrot(float targetx, float targety, float targett);
extern void facepoint(float targetx, float targety);
extern float rotpow;
extern int loopcount;
extern float terror;
extern float deadzonerad;
extern bool inputvov;
extern bool inputvovfp;
extern bool pauseloop;
extern float toplinkp;
extern float toplinkd;

extern float topointloop(float targetx, float targety);
extern float topointloop2(float targetx, float targety);
extern void toPoint(float targX, float targY);
extern void toPointthe2nd(float targX, float targY);
extern void rotarc(float xtar, float ytar, float ttar);
extern void pgrarc(float xcen, float ycen, float arcrad, float arctheta, float gamma, float sigma);
extern float toplinmax;
extern float rotkp;
extern float rotki;
extern float rotkd;