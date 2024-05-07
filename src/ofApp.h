#pragma once

#include "ofMain.h"
#include "ofxJoystick.h"
#include "ofxSvg.h"
#include "StringFormat.h"
#define SAMPLERATE 60

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void updateParam();
    void draw();

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
    
    // カビ01~カビ06
    ofVec2f moldPosition01;
    ofVec2f moldPosition02;
    ofVec2f moldPosition03;
    ofVec2f moldPosition04;
    ofVec2f moldPosition05;
    ofVec2f moldPosition06;
    
    float DegToRad(float rad) {
        return PI * rad / 180.;
    }
		
private:
    ofxJoystick joy_;
    ofxSVG svg;

};
