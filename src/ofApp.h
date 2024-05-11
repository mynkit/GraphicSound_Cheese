#pragma once

#include "ofMain.h"
#include "ofxJoystick.h"
#include "ofxSvg.h"
#include "ofxOsc.h"
#include "StringFormat.h"

#define SAMPLERATE 30
#define HOST "localhost"
#define SCPORT 57110
#define SDPORT 57120

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
    void exit();
    
    // OSC
    ofxOscSender scSender;
    ofxOscSender sdSender;
    
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
    float prevRotaryMechanismTopDegree;
    float rotaryMechanismVelocity;
    
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
    
    vector<int> sawNodeIds = {100, 101, 102, 103, 104};
    vector<float> sawFreqs = {110., 220., 165., 207.652*2, 275.};
    vector<float> sawBpfs = {110., 220.*4., 165.*3., 220*2, 275.*2.};
    vector<float> sawReverbs = {0., 0., 0, 0, 0};
    vector<float> sawParFreqs = {0.022, 0.013, 0.01, 0.05, 0.008};
    vector<float> sawPan2Freqs = {0.03, 0.07, 0.05, 0., 0.03};
    vector<float> sawAmps = {1., 1.25, 1.5, 0.8, 0.5};
		
private:
    ofxJoystick joy_;
    ofxSVG svg;
    
    void playWaterSound(float volume, float theta, float dis) {
        ofxOscMessage m;
        float bubbleSizeMin = 18.;
        float bubbleSizeMax = 100.;
        float rand_ = ofRandom(0.0, 1.0);
        float sustain = ofMap(rand_, 0., 1., 1/bubbleSizeMax, MIN(1/bubbleSizeMin, 0.08)) * 1.0;
        float freq = ofMap(sqrt(rand_), 0., 1., pow(bubbleSizeMax,2), pow(bubbleSizeMin,2));
        float accelerate = ofMap(rand_, 0., 1., sqrt(250/bubbleSizeMax), sqrt(250/bubbleSizeMin));
        float amp = ofMap(rand_, 0., 1., 0.1, 1);
        amp = amp * ofMap(ofRandom(0.0, 1.0)*ofRandom(0.0, 1.0), 0, 1, 0, 1);
        
        if (amp>0.01) {
            m.setAddress("/dirt/play");
            m.addStringArg("s");
            m.addStringArg("simpleSineWave");
            m.addStringArg("freq");
            m.addIntArg(freq);
            m.addStringArg("amp");
            m.addFloatArg(1*volume*amp);
            m.addStringArg("sustain");
            m.addFloatArg(sustain);
            m.addStringArg("accelerate");
            m.addFloatArg(accelerate);
            m.addStringArg("room");
            m.addFloatArg(0.3);
            m.addStringArg("size");
            m.addFloatArg(0.45);
            m.addStringArg("theta");
            m.addFloatArg(theta);
            m.addStringArg("dis");
            m.addFloatArg(dis);
            m.addStringArg("lpf");
            m.addFloatArg(8000);
            m.addStringArg("orbit");
            m.addIntArg(11);
            m.addStringArg("latency");
            m.addFloatArg(0.1);
            sdSender.sendMessage(m, false);
            m.clear();
        }
    }
    
    void playRotateSound(float volume, float theta, float dis) {
        ofxOscMessage m;
        
        float rand_ = ofRandom(0.0, 1.0);
        float freq = 700;
        float sustain = ofMap(rand_, 0, 1, 0.1, 0.08) * ofMap(freq, 1000, 5000, 0.6, 0.5);
        float accelerate = 0.;
        float amp = ofMap(rand_*rand_, 0, 1, 0.1, 1);
        
        if (amp>0.01) {
            m.setAddress("/dirt/play");
            m.addStringArg("s");
            m.addStringArg("simpleSineWave");
            m.addStringArg("freq");
            m.addIntArg(freq);
            m.addStringArg("amp");
            m.addFloatArg(0.4*volume*amp);
            m.addStringArg("sustain");
            m.addFloatArg(sustain);
            m.addStringArg("accelerate");
            m.addFloatArg(accelerate);
            m.addStringArg("room");
            m.addFloatArg(0.1);
            m.addStringArg("size");
            m.addFloatArg(0.1);
            m.addStringArg("theta");
            m.addFloatArg(theta);
            m.addStringArg("dis");
            m.addFloatArg(dis);
            m.addStringArg("orbit");
            m.addIntArg(10);
            m.addStringArg("latency");
            m.addFloatArg(0.1);
            sdSender.sendMessage(m, false);
            m.clear();
        }
    }
    
    void playMoldFallSound(float volume, float freq, float theta, float dis) {
        ofxOscMessage m;
        m.setAddress("/dirt/play");
        m.addStringArg("s");
        m.addStringArg("simpleSineWave");
        m.addStringArg("freq");
        m.addFloatArg(freq*4);
        m.addStringArg("amp");
        m.addFloatArg(volume*0.4);
        m.addStringArg("sustain");
        m.addFloatArg(0.015);
        m.addStringArg("accelerate");
        m.addFloatArg(0.5);
        m.addStringArg("room");
        m.addFloatArg(0.04);
        m.addStringArg("size");
        m.addFloatArg(0);
        m.addStringArg("vibratoFreq");
        m.addFloatArg(8);
        m.addStringArg("vibratoDepth");
        m.addFloatArg(0.24);
        m.addStringArg("theta");
        m.addFloatArg(theta);
        m.addStringArg("lpf");
        m.addFloatArg(1500);
        m.addStringArg("hpf");
        m.addFloatArg(1500);
        m.addStringArg("dis");
        m.addFloatArg(dis);
        m.addStringArg("orbit");
        m.addIntArg(1);
        m.addStringArg("latency");
        m.addFloatArg(0.05);
        sdSender.sendMessage(m, false);
        m.clear();
    }
    
    void playSineSound(float volume, float freq, float sustain, float theta, float dis) {
        ofxOscMessage m;
        m.setAddress("/dirt/play");
        m.addStringArg("s");
        m.addStringArg("simpleSineWave");
        m.addStringArg("freq");
        m.addFloatArg(freq);
        m.addStringArg("amp");
        m.addFloatArg(volume);
        m.addStringArg("sustain");
        m.addFloatArg(sustain);
        m.addStringArg("accelerate");
        m.addFloatArg(0);
        m.addStringArg("room");
        m.addFloatArg(0.3);
        m.addStringArg("size");
        m.addFloatArg(0.5);
        m.addStringArg("vibratoFreq");
        m.addFloatArg(8);
        m.addStringArg("vibratoDepth");
        m.addFloatArg(0.24);
        m.addStringArg("theta");
        m.addFloatArg(theta);
        m.addStringArg("dis");
        m.addFloatArg(dis);
        m.addStringArg("orbit");
        m.addIntArg(2);
        m.addStringArg("latency");
        m.addFloatArg(0.1);
        sdSender.sendMessage(m, false);
        m.clear();
    }
    
    float Max(float a, float b) {
        return (a >= b) ? a : b;
    }
    
    float Min(float a, float b) {
        return (a >= b) ? b : a;
    }

};
