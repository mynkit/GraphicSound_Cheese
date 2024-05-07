#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(SAMPLERATE);
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofEnableArbTex();
    ofEnablePointSprites();
    joy_.setup(GLFW_JOYSTICK_1);
    
    ellipse03TranslateRate = 0.;
    ellipse03TranslateRateDirection = true;
    ellipse03TranslateRateSpeed = 1. * (1. / SAMPLERATE);
    
    rotaryMechanismBottomDegree = 270.;
    rotaryMechanismBottomDegreeSpeed = 0.;
    
    rotaryMechanismTopDegree = 90.;
    rotaryMechanismTopTime = 0;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float ellipse03Translate = ellipse03TranslateRate*38;
    
    float rotaryMechanismBottomX1 = 153.03+28.06*cos(DegToRad(rotaryMechanismBottomDegree));
    float rotaryMechanismBottomY1 = 470.93+28.06*sin(DegToRad(rotaryMechanismBottomDegree));
    float rotaryMechanismBottomX2 = 495.02+28.06*cos(DegToRad(rotaryMechanismBottomDegree));
    float rotaryMechanismBottomY2 = 470.93+28.06*sin(DegToRad(rotaryMechanismBottomDegree));
    
    float rotaryMechanismTopX1 = 454.84+57.81*cos(DegToRad(rotaryMechanismTopDegree));
    float rotaryMechanismTopY1 = 191.45+57.81*sin(DegToRad(rotaryMechanismTopDegree));
    float rotaryMechanismTopX2 = 454.84+57.81*cos(DegToRad(rotaryMechanismTopDegree+180));
    float rotaryMechanismTopY2 = 191.45+57.81*sin(DegToRad(rotaryMechanismTopDegree+180));
    
    
    string svgCode = StringFormat(R"(
    <svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
                              width="%d" height="%d" viewBox="55 70 650.28 665.28" style="enable-background:new 55 70 650.28 665.28;" xml:space="preserve">
    <g id="本体">
        <g>
            <polygon style="fill:#FFFFFF;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" points="75.93,255.12 75.93,546.66 349.43,546.66         "/>
            <polygon style="fill:#FFFFFF;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" points="75.93,255.12 165.86,165.19 439.37,456.72 349.43,546.66         "/>
        </g>
    </g>
    <g id="楕円03">
        <ellipse style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="255.94" cy="258.74" rx="91.61" ry="25.9" transform="translate(%f, %f) "/>
    </g>
    <g id="楕円02">
        <ellipse style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="255.94" cy="238.49" rx="91.61" ry="25.9"/>
    </g>
    <g id="楕円01">
        <ellipse style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="255.94" cy="220" rx="91.61" ry="25.9" />
    </g>
    <g id="車輪左">
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="153.03" cy="470.93" r="45.44"/>
    </g>
    <g id="車輪右">
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="495.02" cy="470.93" r="45.44"/>
    </g>
    <g id="回転機構_x5F_下">
        <g>
            <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="%f" cy="%f" r="6.06" />
            <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="%f" cy="%f" r="6.06" />
            <line style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" x1="%f" y1="%f" x2="%f" y2="%f" />
        </g>
    </g>
    <g id="回転機構_x5F_上">
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="%f" cy="%f" r="24.2"/>
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="%f" cy="%f" r="9.74"/>
        <line style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" x1="%f" y1="%f" x2="%f" y2="%f"/>
    </g>
    <g id="カビ01">
        <circle cx="228.21" cy="89.93" r="12.85"/>
    </g>
    <g id="カビ02">
        <circle cx="258.14" cy="89.93" r="12.85"/>
    </g>
    <g id="カビ03">
        <circle cx="288" cy="89.93" r="12.85"/>
    </g>
    <g id="カビ04">
        <circle cx="228.21" cy="119.37" r="12.85"/>
    </g>
    <g id="カビ05">
        <circle cx="258.14" cy="119.37" r="12.85"/>
    </g>
    <g id="カビ06">
        <circle cx="288" cy="119.37" r="12.85"/>
    </g>
    <g id="煙突">
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="-1338.75" cy="445.9" r="59.95"/>
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="-1284.84" cy="391.99" r="59.95"/>
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="-1230.93" cy="338.07" r="59.95"/>
        <line style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" x1="-1338.75" y1="445.9" x2="-1175.3" y2="282.45"/>
        <rect x="-1323.57" y="47.68" style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" width="50.17" height="90.08"/>
        <rect x="-1416.19" y="92.72" style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" width="50.17" height="90.08"/>
        <polygon style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" points="-1323.57,137.76 -1366.02,182.8 -1366.02,92.72 -1323.57,47.68     "/>
        <path style="fill:#FFFFFF;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" d="M205.66,66.42v64.55c0,0,0,2.04,0,3.09c0,11.07,22.51,20.04,50.28,20.04c27.77,0,50.28-8.97,50.28-20.04
            c0-1.06,0-3.09,0-3.09V66.42"/>
    </g>
    </svg>
    )",int(ofGetWidth()*1.3), int(ofGetHeight()*1.3), ellipse03Translate, ellipse03Translate, rotaryMechanismBottomX1, rotaryMechanismBottomY1, rotaryMechanismBottomX2, rotaryMechanismBottomY1,rotaryMechanismBottomX1,rotaryMechanismBottomY2,rotaryMechanismBottomX2, rotaryMechanismBottomY2,rotaryMechanismTopX1,rotaryMechanismTopY1,rotaryMechanismTopX2,rotaryMechanismTopY2,rotaryMechanismTopX1,rotaryMechanismTopY1,rotaryMechanismTopX2,rotaryMechanismTopY2);
    
    svg.loadFromString(svgCode);
    updateParam();
    
    // chack all button for push
    for (int i = 0; i < joy_.getButtonNum(); i++) {
        if (joy_.isPressed(i)) {
            ofLog() << "pressed :" << i;
        }
        if (joy_.isPushing(i)) {
            ofLog() << "pushing :" << i;
        }
        if (joy_.isRelease(i)) {
            ofLog() << "release :" << i;
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateParam(){
    // 楕円の往復
    if (ellipse03TranslateRateDirection) {
        if (1 - ellipse03TranslateRate >= ellipse03TranslateRateSpeed) {
            ellipse03TranslateRate += ellipse03TranslateRateSpeed;
        } else {
            ellipse03TranslateRate = 1.;
            ellipse03TranslateRateDirection = false;
        }
    } else {
        if (ellipse03TranslateRate >= ellipse03TranslateRateSpeed) {
            ellipse03TranslateRate -= ellipse03TranslateRateSpeed;
        } else {
            ellipse03TranslateRate = 0.;
            ellipse03TranslateRateDirection = true;
        }
    }
    
    // 回転機構下
    rotaryMechanismBottomDegree += rotaryMechanismBottomDegreeSpeed;
    if (rotaryMechanismBottomDegree>=360) {rotaryMechanismBottomDegree-=360.;}
    
    // 回転機構上
    if (rotaryMechanismTopTime <= 5) {
        rotaryMechanismTopDegree = ofMap(rotaryMechanismTopTime, 0, 5, 90, -90);
    } else if (rotaryMechanismTopTime <= 23) {
        rotaryMechanismTopDegree = ofMap(rotaryMechanismTopTime, 5, 23, -90, -90-720);
    } else if (rotaryMechanismTopTime <= 33) {
        rotaryMechanismTopDegree = ofMap(rotaryMechanismTopTime, 23, 33, -90, -90-360);
    } else if (rotaryMechanismTopTime <= 45) {
        float seed = ofMap(rotaryMechanismTopTime, 33, 45, 0., 1.);
        rotaryMechanismTopDegree = ofMap(pow(seed,0.95), 0., 1., -90, -90-350);
    } else if (rotaryMechanismTopTime <= 56) {
        float seed = ofMap(rotaryMechanismTopTime, 45, 56, 0., 1.);
        rotaryMechanismTopDegree = ofMap(pow(seed,0.9), 0., 1., -80, -90-180);
    } else if (rotaryMechanismTopTime <= 80) {
        rotaryMechanismTopDegree = 90 - 30 * sin((PI/8.) * (rotaryMechanismTopTime-56));
    } else {
        rotaryMechanismTopDegree = 90;
    }
    
    rotaryMechanismTopTime += 1;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255, 0);
    ofSetCurveResolution(120);
    svg.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
