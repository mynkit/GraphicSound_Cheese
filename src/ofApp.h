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
    
    bool ellipse03Move;
    float ellipse03TranslateRate;
    float ellipse03TranslateRateDirection;
    float ellipse03TranslateRateSpeed;
    
    float rotaryMechanismBottomDegree;
    float rotaryMechanismBottomDegreeSpeed;
    
    float rotaryMechanismTopDegree;
    int rotaryMechanismTopTime;
    int initRotaryMechanismTopTime;
    
    float DegToRad(float rad) {
        return PI * rad / 180.;
    }
		
private:
    ofxJoystick joy_;
    ofxSVG svg;

};
