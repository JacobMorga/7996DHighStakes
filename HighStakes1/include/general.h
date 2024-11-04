using namespace std;

extern const float pi;
extern const float wheelEncoderRatio;
extern const float wheelDia;
extern float getDir(float input);
extern float getAngle(void);

extern const bool on;
extern const bool off;
extern const bool pressed;
extern const bool unpressed;

class DisplayBox{
    public:
    float X1 = 0;
    float Y1 = 0;
    float X2 = 0;
    float Y2 = 0;
    float AmtX = 0;
    float AmtY = 0;
    bool hasButtons = false;
    vector<int> buttonColors {};
    bool multiSelect {};

    DisplayBox (float inX1, float inY1, float inX2, float inY2, float inAmtX, float inAmtY);
    void setButtons (vector<int> inColors, bool inMultiSelect);
    void drawVerticalLines ();
    void drawHorizontalLines ();
    void handleButtons ();
};
extern void autonSelectorNEW (vector<DisplayBox> divisions);