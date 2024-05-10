#pragma once

#include "ofMain.h"
#include "ofxJoystick.h"
#include "ofxSvg.h"
#include "StringFormat.h"
#define SAMPLERATE 30

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void updateParam();
    void draw();
    void initMold();
    void randomMoldFall();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    bool updateParamStop = false;
    
    // 楕円03
    bool ellipse03Move;
    float ellipse03TranslateRate;
    float ellipse03TranslateRateDirection;
    float ellipse03TranslateRateSpeed;
    
    // 回転機構_x5F_下
    bool rotaryMechanismBottomMove;
    float rotaryMechanismBottomDegree;
    float rotaryMechanismBottomDegreeSpeed;
    float rotaryMechanismBottomDegreeMaxSpeed;
    float rotaryMechanismBottomDegreeAccelerationAdd;
    float rotaryMechanismBottomDegreeAccelerationMulti;
    
    // 回転機構_x5F_上
    float rotaryMechanismTopDegree;
    int rotaryMechanismTopTime;
    int initRotaryMechanismTopTime;
    
    // カビ01~カビ09
    ofVec2f mold01Position;
    ofVec2f mold02Position;
    ofVec2f mold03Position;
    ofVec2f mold04Position;
    ofVec2f mold05Position;
    ofVec2f mold06Position;
    ofVec2f mold07Position;
    ofVec2f mold08Position;
    ofVec2f mold09Position;
    int mold01Time;
    int mold02Time;
    int mold03Time;
    int mold04Time;
    int mold05Time;
    int mold06Time;
    int mold07Time;
    int mold08Time;
    int mold09Time;
    int mold01Duration;
    int mold02Duration;
    int mold03Duration;
    int mold04Duration;
    int mold05Duration;
    int mold06Duration;
    int mold07Duration;
    int mold08Duration;
    int mold09Duration;
    float mold01SinkDepth;
    float mold01SinkY;
    float mold06SinkDepth;
    float mold06SinkY;
    float mold07SinkDepth;
    float mold07SinkY;
    float mold09SinkDepth;
    float mold09SinkY;
    
    float DegToRad(float rad) {
        return PI * rad / 180.;
    }
		
private:
    ofxJoystick joy_;
    ofxSVG svg;

};
