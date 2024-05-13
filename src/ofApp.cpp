#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(SAMPLERATE);
    ofEnableAntiAliasing();
    // ofEnableSmoothing();
    // ofEnableArbTex();
    // ofEnablePointSprites();
    
    // OSCのセッティング
    scSender.setup(HOST, SCPORT);
    sdSender.setup(HOST, SDPORT);
    
    // GamePadのセットアップ
    joy_.setup(GLFW_JOYSTICK_1);
    
    // 楕円03
    ellipse03Move = false;
    ellipse03TranslateRate = 0.;
    ellipse03TranslateRateDirection = true;
    ellipse03TranslateRateSpeed = 1. * (1. / SAMPLERATE);
    
    // 回転機構_x5F_下
    rotaryMechanismBottomMove = false;
    rotaryMechanismBottomDegree = 270.;
    rotaryMechanismBottomDegreeSpeed = 0.;
    rotaryMechanismBottomDegreeMaxSpeed = 50. * (30. / SAMPLERATE);
    rotaryMechanismBottomDegreeAccelerationAdd = 0.05 * (60. / SAMPLERATE);
    rotaryMechanismBottomDegreeAccelerationMulti = pow(1.01, 60. / SAMPLERATE);
    
    // 回転機構_x5F_上
    rotaryMechanismTopDegree = 90.;
    initRotaryMechanismTopTime = 80 * (SAMPLERATE / 30);
    rotaryMechanismTopTime = initRotaryMechanismTopTime;
    
    // カビ01~カビ06
    mold01Position = ofVec2f(243, 140);
    mold02Position = ofVec2f(271, 137);
    mold03Position = ofVec2f(0, 100);
    mold04Position = ofVec2f(0, 100);
    mold05Position = ofVec2f(0, 100);
    mold06Position = ofVec2f(0, 100);
    mold07Position = ofVec2f(0, 100);
    mold08Position = ofVec2f(0, 100);
    mold09Position = ofVec2f(0, 100);
    
    mold01Duration = 131 * (SAMPLERATE / 30); // バウンドして回転機構にぶつかって沈む
    mold02Duration = 48 * (SAMPLERATE / 30); // バウンドしてそのまま消える
    mold03Duration = 46 * (SAMPLERATE / 30); // 03と04は重なってるやつ
    mold04Duration = 46 * (SAMPLERATE / 30);
    mold05Duration = 66 * (SAMPLERATE / 30); // 05はそのまま下に落ちる
    mold06Duration = 95 * (SAMPLERATE / 30); // 落ちてきてそのまま沈む
    mold07Duration = 88 * (SAMPLERATE / 30); // 左端で落ちてきてそのまま沈む
    mold08Duration = 42 * (SAMPLERATE / 30); // 回転機構に叩かれるやつ
    mold09Duration = 87 * (SAMPLERATE / 30); // 最後のやつ
    
    mold01Time = mold01Duration + 1;
    mold02Time = mold02Duration + 1;
    mold03Time = mold03Duration + 1;
    mold04Time = mold04Duration + 1;
    mold05Time = mold05Duration + 1;
    mold06Time = mold06Duration + 1;
    mold07Time = mold07Duration + 1;
    mold08Time = mold08Duration + 1;
    mold09Time = mold09Duration + 1;
    
    mold01SinkDepth = 0.;
    mold06SinkDepth = 0.;
    mold07SinkDepth = 0.;
    mold09SinkDepth = 0.;
    
    chimneyVibrate = false;
    chimneyDuration = 30 * (SAMPLERATE / 30);
    chimneyTime = chimneyDuration + 1;
    chimneyVibrateX = 0.;
    chimneyVibrateXRange = 3;
    
    // sawの準備
    ofxOscMessage m;
    for (size_t i = 0; i < sawNodeIds.size(); ++i) {
        m.setAddress("/n_free");
        m.addIntArg(sawNodeIds[i]);
        scSender.sendMessage(m, false);
        m.clear();
        m.setAddress("/s_new");
        m.addStringArg("sawDry");
        m.addIntArg(sawNodeIds[i]);
        m.addIntArg(1);
        m.addIntArg(0);
        m.addStringArg("amp");
        m.addFloatArg(sawAmps[i]*0.);
        m.addStringArg("freq");
        m.addFloatArg(sawFreqs[i]);
        m.addStringArg("bpf");
        m.addFloatArg(sawBpfs[i]);
        m.addStringArg("reverb");
        m.addFloatArg(sawReverbs[i]);
        m.addStringArg("parFreqs");
        m.addFloatArg(sawParFreqs[i]);
        m.addStringArg("pan2Freqs");
        m.addFloatArg(sawPan2Freqs[i]);
        scSender.sendMessage(m, false);
        m.clear();
    }
    // sineの準備
    for (size_t i = 0; i < sineNodeIds.size(); ++i) {
        m.setAddress("/n_free");
        m.addIntArg(sineNodeIds[i]);
        scSender.sendMessage(m, false);
        m.clear();
        m.setAddress("/s_new");
        m.addStringArg("sineDry");
        m.addIntArg(sineNodeIds[i]);
        m.addIntArg(1);
        m.addIntArg(0);
        m.addStringArg("amp");
        m.addFloatArg(0.);
        m.addStringArg("freq");
        m.addFloatArg(sineFreqs[i]);
        m.addStringArg("parFreq");
        m.addFloatArg(sineParFreqs[i]);
        m.addStringArg("pan2Freq");
        m.addFloatArg(sinePan2Freqs[i]);
        m.addStringArg("vibratoFreq");
        m.addFloatArg(sineVibratoFreq);
        m.addStringArg("vibratoDepth");
        m.addFloatArg(sineVibratoDepth);
        scSender.sendMessage(m, false);
        m.clear();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // chack all button for push
    for (int i = 0; i < joy_.getButtonNum(); i++) {
        if (joy_.isPressed(i)) {
//            ofLog() << "pressed :" << i;
            if (i==0) {
                // 青のXボタン押
                ellipse03Move = !ellipse03Move;
            }
            if (i==1) {
                // 赤のAボタン押
                if (chimneyTime>=chimneyDuration) chimneyTime = 0;
            }
            if (i==2) {
                // 黄のBボタン押
                randomMoldFall();
            }
            if (i==3) {
                // 緑のYボタン押
                rotaryMechanismBottomMove = true;
            }
            if (i==4 || i==5) {
                // LRボタン押
                rotaryMechanismTopTime = 0;
            }
            if (i==9) {
                // STARTボタン押
                updateParamStop = !updateParamStop;
            }
        }
        if (joy_.isPushing(i)) {
//            ofLog() << "pushing :" << i;
        }
        if (joy_.isRelease(i)) {
//            ofLog() << "release :" << i;
            if (i==3) {
                // 緑のYボタン離
                rotaryMechanismBottomMove = false;
            }
        }
    }
    
    if (updateParamStop) {
        backgroundColor = pausedBackgroundColor;
        // PAUSEモードのときはシンセの音量0にする
        ofxOscMessage m;
        for (size_t i = 0; i < sawNodeIds.size(); ++i) {
            m.setAddress("/n_set");
            m.addIntArg(sawNodeIds[i]);
            m.addStringArg("amp");
            m.addFloatArg(0.);
            scSender.sendMessage(m, false);
            m.clear();
        }
        for (size_t i = 0; i < sineNodeIds.size(); ++i) {
            m.setAddress("/n_set");
            m.addIntArg(sineNodeIds[i]);
            m.addStringArg("amp");
            m.addFloatArg(0.);
            scSender.sendMessage(m, false);
            m.clear();
        }
    } else {
        backgroundColor = defaultBackgroundColor;
    }
    
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
            <polygon style="fill:rgb(%d,%d,%d);stroke:#000000;stroke-width:1;stroke-miterlimit:10;" points="75.93,255.12 75.93,546.66 349.43,546.66         "/>
            <polygon style="fill:rgb(%d,%d,%d) ;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" points="75.93,255.12 165.86,165.19 439.37,456.72 349.43,546.66         "/>
        </g>
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
    <g id="カビ07">
        <circle cx="%f" cy="%f" r="12.85"/>
        <rect x="%f" y="%f" width="25.7" height="%f" style="fill:rgb(%d,%d,%d) "/>
    </g>
    <g id="カビ09">
        <circle cx="%f" cy="%f" r="12.85"/>
        <rect x="%f" y="%f" width="25.7" height="%f" style="fill:rgb(%d,%d,%d) "/>
    </g>
    <g id="カビ01">
        <circle cx="%f" cy="%f" r="12.85"/>
        <rect x="%f" y="%f" width="25.7" height="%f" style="fill:rgb(%d,%d,%d) "/>
    </g>
    <g id="カビ02">
        <circle cx="%f" cy="%f" r="12.85"/>
    </g>
    <g id="カビ03">
        <circle cx="%f" cy="%f" r="12.85"/>
    </g>
    <g id="カビ04">
        <circle cx="%f" cy="%f" r="12.85"/>
    </g>
    <g id="カビ05">
        <circle cx="%f" cy="%f" r="12.85"/>
    </g>
    <g id="カビ06">
        <circle cx="%f" cy="%f" r="12.85"/>
        <rect x="%f" y="%f" width="25.7" height="%f" style="fill:rgb(%d,%d,%d) "/>
    </g>
    <g id="カビ08">
        <circle cx="%f" cy="%f" r="12.85"/>
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
    <g id="煙突">
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="%f" cy="445.9" r="59.95"/>
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="%f" cy="391.99" r="59.95"/>
        <circle style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" cx="%f" cy="338.07" r="59.95"/>
        <line style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" x1="%f" y1="445.9" x2="%f" y2="282.45"/>
        <rect x="%f" y="47.68" style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" width="50.17" height="90.08"/>
        <rect x="%f" y="92.72" style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" width="50.17" height="90.08"/>
        <polygon style="fill:none;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" points="%f,137.76 %f,182.8 %f,92.72 %f,47.68     "/>
        <path transform="translate(%f, 0) " style="fill:rgb(%d,%d,%d) ;stroke:#000000;stroke-width:1;stroke-miterlimit:10;" d="M205.66,66.42v64.55c0,0,0,2.04,0,3.09c0,11.07,22.51,20.04,50.28,20.04c27.77,0,50.28-8.97,50.28-20.04
            c0-1.06,0-3.09,0-3.09V66.42"/>
    </g>
    </svg>
    )",int(ofGetHeight()*1.3), int(ofGetHeight()*1.3), backgroundColor,backgroundColor,backgroundColor, backgroundColor,backgroundColor,backgroundColor, rotaryMechanismBottomX1, rotaryMechanismBottomY1, rotaryMechanismBottomX2, rotaryMechanismBottomY1,rotaryMechanismBottomX1,rotaryMechanismBottomY2,rotaryMechanismBottomX2, rotaryMechanismBottomY2,rotaryMechanismTopX1,rotaryMechanismTopY1,rotaryMechanismTopX2,rotaryMechanismTopY2,rotaryMechanismTopX1,rotaryMechanismTopY1,rotaryMechanismTopX2,rotaryMechanismTopY2,mold07Position.x,mold07Position.y,mold07Position.x-12.85,mold07SinkY+12.85,mold07SinkDepth,backgroundColor,backgroundColor,backgroundColor,mold09Position.x,mold09Position.y,mold09Position.x-12.85,mold09SinkY+12.85,mold09SinkDepth,backgroundColor,backgroundColor,backgroundColor,mold01Position.x,mold01Position.y,mold01Position.x-12.85,mold01SinkY+12.85,mold01SinkDepth,backgroundColor,backgroundColor,backgroundColor,mold02Position.x,mold02Position.y,mold03Position.x,mold03Position.y,mold04Position.x,mold04Position.y,mold05Position.x,mold05Position.y,mold06Position.x,mold06Position.y,mold06Position.x-12.85,mold06SinkY+12.85,mold06SinkDepth,backgroundColor,backgroundColor,backgroundColor,mold08Position.x,mold08Position.y, ellipse03Translate, ellipse03Translate, -1338.75+chimneyVibrateX,-1284.84+chimneyVibrateX,-1230.93+chimneyVibrateX,-1338.75+chimneyVibrateX,-1175.3+chimneyVibrateX,-1323.57+chimneyVibrateX,-1416.19+chimneyVibrateX,-1323.57+chimneyVibrateX,-1366.02+chimneyVibrateX,-1366.02+chimneyVibrateX,-1323.57+chimneyVibrateX,chimneyVibrateX,  backgroundColor,backgroundColor,backgroundColor);
    
    svg.loadFromString(svgCode);
    if (!updateParamStop) updateParam();
    
}

//--------------------------------------------------------------
void ofApp::updateParam(){
    // 楕円の往復
    if (ellipse03Move || (ellipse03TranslateRate>0 && ellipse03TranslateRate<1)) {
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
    }
    
    // 回転機構下
    if (rotaryMechanismBottomMove) {
        rotaryMechanismBottomDegreeSpeed += rotaryMechanismBottomDegreeAccelerationAdd;
        rotaryMechanismBottomDegreeSpeed *= rotaryMechanismBottomDegreeAccelerationMulti;
    } else {
        rotaryMechanismBottomDegreeSpeed -= rotaryMechanismBottomDegreeAccelerationAdd;
        rotaryMechanismBottomDegreeSpeed /= rotaryMechanismBottomDegreeAccelerationMulti;
    }
    if (rotaryMechanismBottomDegreeSpeed > rotaryMechanismBottomDegreeMaxSpeed) {
        // 回転速度の最大値指定
        rotaryMechanismBottomDegreeSpeed = rotaryMechanismBottomDegreeMaxSpeed;
    }
    if (rotaryMechanismBottomDegreeSpeed < 0) {
        // 回転速度の最小値指定
        rotaryMechanismBottomDegreeSpeed = 0;
    }
    rotaryMechanismBottomDegree += rotaryMechanismBottomDegreeSpeed;
    if (rotaryMechanismBottomDegree>=360) {
        rotaryMechanismBottomDegree-=360.;
        playSineSound(0.2, 164.814, 0.03, 0, 1);
    }
    
    
    ofxOscMessage m;
    float sawSynthVol = 0.8;
    for (size_t i = 0; i < sawNodeIds.size(); ++i) {
        m.setAddress("/n_set");
        m.addIntArg(sawNodeIds[i]);
        m.addStringArg("amp");
        if (i==3) {
            float seed = ofMap(rotaryMechanismBottomDegreeSpeed, 0., rotaryMechanismBottomDegreeMaxSpeed, 0., 1);
            m.addFloatArg(sawSynthVol*0.8*sawAmps[i]*pow(seed, 2.));
        } else {
            float minVol = 0.;
            m.addFloatArg(sawSynthVol*0.8*sawAmps[i]*ofMap(rotaryMechanismBottomDegreeSpeed, 0., rotaryMechanismBottomDegreeMaxSpeed, minVol, 0.8));
        }
        m.addStringArg("freq");
        m.addFloatArg(sawFreqs[i]*329.628/440.);
        scSender.sendMessage(m, false);
        m.clear();
    }
    float sineSynthVol = 0.8;
    float sineAmp = 0.;
    if (ellipse03Move) {
        sineAmp = 0.12;
    }
    for (size_t i = 0; i < sineNodeIds.size(); ++i) {
        m.setAddress("/n_set");
        m.addIntArg(sineNodeIds[i]);
        m.addStringArg("amp");
        m.addFloatArg(sineSynthVol*sineAmp);
        scSender.sendMessage(m, false);
        m.clear();
    }
    
    // 回転機構下によって球が出される
    if (rotaryMechanismBottomDegreeSpeed >= 14.2 && rotaryMechanismBottomMove) {
        initMold();
    }
    
    // 回転機構上
    if (rotaryMechanismTopTime <= initRotaryMechanismTopTime) {
        rotaryMechanismTopTime += 1;
    }
    
    prevRotaryMechanismTopDegree = rotaryMechanismTopDegree;
    
    if (rotaryMechanismTopTime <= 5 * (SAMPLERATE / 30)) {
        rotaryMechanismTopDegree = ofMap(rotaryMechanismTopTime, 0, 5 * (SAMPLERATE / 30), 90, -90);
    } else if (rotaryMechanismTopTime <= 23 * (SAMPLERATE / 30)) {
        rotaryMechanismTopDegree = ofMap(rotaryMechanismTopTime, 5 * (SAMPLERATE / 30), 23 * (SAMPLERATE / 30), -90, -90-720);
    } else if (rotaryMechanismTopTime <= 33 * (SAMPLERATE / 30)) {
        rotaryMechanismTopDegree = ofMap(rotaryMechanismTopTime, 23 * (SAMPLERATE / 30), 33 * (SAMPLERATE / 30), -90, -90-360);
    } else if (rotaryMechanismTopTime <= 45 * (SAMPLERATE / 30)) {
        float seed = ofMap(rotaryMechanismTopTime, 33 * (SAMPLERATE / 30), 45 * (SAMPLERATE / 30), 0., 1.);
        rotaryMechanismTopDegree = ofMap(pow(seed,0.95), 0., 1., -90, -90-350);
    } else if (rotaryMechanismTopTime <= 56 * (SAMPLERATE / 30)) {
        float seed = ofMap(rotaryMechanismTopTime, 45 * (SAMPLERATE / 30), 56 * (SAMPLERATE / 30), 0., 1.);
        rotaryMechanismTopDegree = ofMap(pow(seed,0.75), 0., 1., -80, -90-180);
    } else if (rotaryMechanismTopTime <= 80 * (SAMPLERATE / 30)) {
        rotaryMechanismTopDegree = 90 - 30 * sin((PI/(8. * (SAMPLERATE / 30.))) * (rotaryMechanismTopTime-(56. * (SAMPLERATE / 30))));
    } else {
        rotaryMechanismTopDegree = 90;
    }
    
    rotaryMechanismVelocity = (rotaryMechanismTopDegree - prevRotaryMechanismTopDegree) * (SAMPLERATE / 30);
    
    // 回転に音つける
    if (abs(rotaryMechanismVelocity) > 0.01) {
        playRotateSound(pow(Min(ofMap(abs(rotaryMechanismVelocity), 0., 60., 0., 1.), 0.3), 0.7), 0.3, 1);
    }
    
    // カビがでてくるときの音
    
    if (chimneyTime==(int)(chimneyDuration*0.6)) {
        // 煙突の振動でカビが全部落ちる
        if(mold01Time >= mold01Duration) mold01Time = (int)ofRandom(-15, 0);
        if(mold02Time >= mold02Duration) mold02Time = (int)ofRandom(-15, 0);
        if(mold03Time >= mold03Duration) mold03Time = (int)ofRandom(-15, 0);
        if(mold04Time >= mold04Duration) mold04Time = (int)ofRandom(-15, 0);
        if(mold05Time >= mold05Duration) mold05Time = (int)ofRandom(-15, 0);
        if(mold06Time >= mold06Duration) mold06Time = (int)ofRandom(-15, 0);
        if(mold07Time >= mold07Duration) mold07Time = (int)ofRandom(-15, 0);
        if(mold09Time >= mold09Duration) mold09Time = (int)ofRandom(-15, 0);
    }
    
    if (mold01Time == 0) {
        playMoldFallSound(0.8, 440, -0.05, 1);
    }
    if (mold02Time == 0) {
        playMoldFallSound(0.8, 220, 0.0, 1);
    }
    if (mold03Time == 0) {
        playMoldFallSound(0.8, 440, -0.1, 1);
    }
    if (mold04Time == 0) {
        playMoldFallSound(0.8, 220, -0.1, 1);
    }
    if (mold05Time == 0) {
        playMoldFallSound(0.8, 220, -0.025, 1);
    }
    if (mold06Time == 0) {
        playMoldFallSound(0.8, 220, 0.1, 1);
    }
    if (mold07Time == 0) {
        playMoldFallSound(0.8, 220, -0.2, 1);
    }
    if (mold08Time == 0) {
        playMoldFallSound(0.8, 277.183, 0.1, 1);
    }
    if (mold09Time == 0) {
        playMoldFallSound(0.8, 440, 0.05, 1);
    }
    
    // 回転機構_上に当たるときの音
    if (mold01Time == 33 * (SAMPLERATE / 30)) {
        playMoldAttackSound(0.092, 1479.978, 0.2, 1);
    }
    if (mold08Time == 29 * (SAMPLERATE / 30) && abs(rotaryMechanismVelocity)>0) {
//        cout << rotaryMechanismTopDegree << endl;
        playMoldAttackSound(0.08, 1108.731, 0.2, 1);
    }
    
    // チーズに跳ね返る音
    if (mold01Time == 16 * (SAMPLERATE / 30)) {
        playMoldReboundSound(0.15, 207.652, 0.0, 1);
    }
    if (mold02Time == 16 * (SAMPLERATE / 30)) {
        playMoldReboundSound(0.15, 184.997, 0.0, 1);
    }
    if (mold03Time == 14 * (SAMPLERATE / 30)) {
        playMoldReboundSound(0.15, 146.832, -0.1, 1);
    }
    if (mold04Time == 14 * (SAMPLERATE / 30)) {
        playMoldReboundSound(0.15, 207.652, -0.1, 1);
    }
    if (mold08Time == 12 * (SAMPLERATE / 30)) {
        playMoldReboundSound(0.15, 184.997, 0.1, 1);
    }
    
    // カビ01
    if (mold01Time >= 63 * (SAMPLERATE / 30)) {
        mold01SinkDepth = 25.7;
        mold01SinkY = 0.0052*pow(232 - 367, 2) - 0.0052*pow(422 - 367, 2) + 0.0018*pow(422 - 440, 2) - 0.0018*pow(243 - 440, 2) + 335;
    } else {
        mold01SinkDepth = 0.;
    }
    
    if (mold01Time <= 16 * (SAMPLERATE / 30)) {
        mold01Position.x = 243;
        mold01Position.y = ofMap(mold01Time, 0, 16 * (SAMPLERATE / 30), 140, 335);
    } else if (mold01Time <= 33 * (SAMPLERATE / 30)) {
        mold01Position.x = ofMap(mold01Time, 16 * (SAMPLERATE / 30), 33 * (SAMPLERATE / 30), 243, 422);
        mold01Position.y = 0.0018*pow(mold01Position.x - 440, 2) - 0.0018*pow(243 - 440, 2) + 335;
        if (rotaryMechanismTopTime >= initRotaryMechanismTopTime && mold01Time == 33 * (SAMPLERATE / 30)) {
            rotaryMechanismTopTime = 0;
        }
    } else if (mold01Time <= 63 * (SAMPLERATE / 30)) {
        float seed = ofMap(mold01Time, 33 * (SAMPLERATE / 30), 63 * (SAMPLERATE / 30), 0., 1.);
        mold01Position.x = ofMap(pow(seed, 0.8), 0., 1., 422, 240);
        mold01Position.y = 0.006*pow(mold01Position.x - 367, 2) - 0.006*pow(422 - 367, 2) + 0.0018*pow(422 - 440, 2) - 0.0018*pow(243 - 440, 2) + 335;
    } else if (mold01Time <= 65 * (SAMPLERATE / 30)) {
        mold01Position.x = ofMap(mold01Time, 63 * (SAMPLERATE / 30), 65 * (SAMPLERATE / 30), 240, 236);
        mold01Position.y = ofMap(mold01Time, 63 * (SAMPLERATE / 30), 65 * (SAMPLERATE / 30), mold01SinkY, mold01SinkY + 13);
    } else if (mold01Time <= mold01Duration) {
        mold01Position.y = ofMap(mold01Time, 65 * (SAMPLERATE / 30), mold01Duration, mold01SinkY + 13, mold01SinkY + 25.7);
        playMoldSinkSound(0.3, -0.25, 1);
    }
    // カビ02
    if (mold02Time <= 16 * (SAMPLERATE / 30)) {
        mold02Position.x = 271;
        mold02Position.y = ofMap(mold02Time, 0, 16 * (SAMPLERATE / 30), 137, 360);
    } else if (mold02Time <= 48 * (SAMPLERATE / 30)) {
        mold02Position.x = ofMap(mold02Time, 16 * (SAMPLERATE / 30), 47 * (SAMPLERATE / 30), 271, 559);
        mold02Position.y = 0.0022*pow(mold02Position.x - 438, 2) - 0.0022*pow(271 - 438, 2) + 360;
    }
    
    // カビ03
    if (mold03Time <= 14 * (SAMPLERATE / 30)) {
        mold03Position.x = 225;
        mold03Position.y = ofMap(mold03Time, 0, 14 * (SAMPLERATE / 30), 140, 362);
    } else if (mold03Time <= mold03Duration) {
        mold03Position.x = ofMap(mold03Time, 14 * (SAMPLERATE / 30), mold03Duration, 225, 569);
        mold03Position.y = 0.0035*pow(mold03Position.x - 290, 2) - 0.0035*pow(225 - 290, 2) + 362;
    }
    
    // カビ04
    if (mold04Time <= 14 * (SAMPLERATE / 30)) {
        mold04Position.x = 225;
        mold04Position.y = ofMap(mold04Time, 0, 14 * (SAMPLERATE / 30), 140, 362);
    } else if (mold04Time <= mold04Duration) {
        mold04Position.x = ofMap(mold04Time, 14 * (SAMPLERATE / 30), mold04Duration, 225, 575);
        mold04Position.y = 0.002*pow(mold04Position.x - 360, 2) - 0.002*pow(225 - 360, 2) + 362;
    }
    
    // カビ05
    if (mold05Time <= mold05Duration) {
        mold05Position.x = 247;
        mold05Position.y = ofMap(mold05Time, 0, mold05Duration, 138, 595);
    }
    
    // カビ06
    if (mold06Time >= 29 * (SAMPLERATE / 30)) {
        mold06SinkDepth = 25.7;
        mold06SinkY = 372;
    } else {
        mold06SinkDepth = 0.;
    }
    
    if (mold06Time <= 28 * (SAMPLERATE / 30)) {
        mold06Position.x = 284;
        mold06Position.y = ofMap(mold06Time, 0, 28 * (SAMPLERATE / 30), 137, 370);
    } else if (mold06Time <= 29 * (SAMPLERATE / 30)) {
        mold06Position.y = 374;
    } else if (mold06Time <= mold06Duration) {
        mold06Position.y = ofMap(mold06Time, 29 * (SAMPLERATE / 30), mold06Duration, mold06SinkY, mold06SinkY + 25.7);
        playMoldSinkSound(0.3, 0.1, 1);
    }
    
    // カビ07
    if (mold07Time >= 19 * (SAMPLERATE / 30)) {
        mold07SinkDepth = 25.7;
        mold07SinkY = 292;
    } else {
        mold07SinkDepth = 0.;
    }
    
    if (mold07Time <= 19 * (SAMPLERATE / 30)) {
        mold07Position.x = 218;
        mold07Position.y = ofMap(mold07Time, 0, 18 * (SAMPLERATE / 30), 137, 284);
    } else if (mold07Time <= mold07Duration) {
        mold07Position.y = ofMap(mold07Time, 19 * (SAMPLERATE / 30), mold07Duration, mold07SinkY, mold07SinkY + 25.7);
        playMoldSinkSound(0.3, -0.2, 1);
    }
    
    // カビ08
    if (mold08Time <= 12 * (SAMPLERATE / 30)) {
        mold08Position.x = 246;
        mold08Position.y = ofMap(mold08Time, 0, 12 * (SAMPLERATE / 30), 140, 284);
    } else if (mold08Time <= 29 * (SAMPLERATE / 30)) {
        mold08Position.x = ofMap(mold08Time, 12 * (SAMPLERATE / 30), 29 * (SAMPLERATE / 30), 246, 405);
        mold08Position.y = 0.0015*pow(mold08Position.x - 450, 2) - 0.0015*pow(246 - 450, 2) + 284;
    } else if (mold08Time <= mold08Duration) {
        if (abs(rotaryMechanismVelocity)>0) {
            mold08Position.x = ofMap(mold08Time, 29 * (SAMPLERATE / 30), mold08Duration, 405, 600);
            mold08Position.y = 0.007*pow(mold08Position.x - 400, 2) - 0.007*pow(405 - 400, 2) + 0.0015*pow(405 - 450, 2) - 0.0015*pow(246 - 450, 2) + 284;
        } else {
            mold08Position.x = ofMap(mold08Time, 29 * (SAMPLERATE / 30), mold08Duration, 405, 600);
            mold08Position.y = 0.0015*pow(mold08Position.x - 450, 2) - 0.0015*pow(246 - 450, 2) + 284;
        }
    }
    
    // カビ09
    if (mold09Time >= 21 * (SAMPLERATE / 30)) {
        mold09SinkDepth = 25.7;
        mold09SinkY = 318;
    } else {
        mold09SinkDepth = 0.;
    }
    
    if (mold09Time <= 21 * (SAMPLERATE / 30)) {
        mold09Position.x = 274;
        mold09Position.y = ofMap(mold09Time, 0, 21 * (SAMPLERATE / 30), 139, 320);
    } else if (mold09Time <= mold09Duration) {
        mold09Position.y = ofMap(mold09Time, 21 * (SAMPLERATE / 30), mold09Duration, 320, 343.7);
        playMoldSinkSound(0.3, 0.05, 1);
    }
    
    // 煙突
    if (chimneyTime == 0) {
        playChimneyVibrateSound(0.3, 0.0, 1);
    }
    
    // 時刻更新
    if (mold01Time <= mold01Duration) {
        mold01Time += 1;
    }
    if (mold02Time <= mold02Duration) {
        mold02Time += 1;
    }
    if (mold03Time <= mold03Duration) {
        mold03Time += 1;
    }
    if (mold04Time <= mold04Duration) {
        mold04Time += 1;
    }
    if (mold05Time <= mold05Duration) {
        mold05Time += 1;
    }
    if (mold06Time <= mold06Duration) {
        mold06Time += 1;
    }
    if (mold07Time <= mold07Duration) {
        mold07Time += 1;
    }
    if (mold08Time <= mold08Duration) {
        mold08Time += 1;
    }
    if (mold09Time <= mold09Duration) {
        mold09Time += 1;
    }
    
    if (chimneyTime <= chimneyDuration) {
        chimneyTime += 1;
        chimneyVibrateX = chimneyVibrateXRange;
        chimneyVibrateXRange *= -1;
    } else {
        chimneyVibrateX = 0;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(backgroundColor, 0);
//    ofSetCurveResolution(120);
    float margin = (ofGetWidth()-ofGetHeight())/2.;
    ofTranslate(margin, 0);
    svg.draw();
    
    if (updateParamStop) {
        ofSetColor(255);
        string pauseTxt = "PAUSE";
        ofRectangle pauseTxtSize = getBitmapStringBoundingBox(pauseTxt);
        ofDrawBitmapString(pauseTxt, ofGetWidth()*0.5 - pauseTxtSize.width*0.5, ofGetHeight()*0.5 - pauseTxtSize.height*0.5);
    }

    // yohaku
    ofTranslate(-margin, 0);
    ofRectangle marginLeft;
    marginLeft.x = 0;
    marginLeft.y = 0;
    marginLeft.width = margin;
    marginLeft.height = ofGetHeight();
    
    ofRectangle marginRight;
    marginRight.x = margin + ofGetHeight();
    marginRight.y = 0;
    marginRight.width = margin;
    marginRight.height = ofGetHeight();

    ofSetColor(255);
    ofDrawRectangle(marginLeft);
    ofDrawRectangle(marginRight);

}

//--------------------------------------------------------------
void ofApp::initMold(){
    if(mold01Time >= mold01Duration) mold01Time = 0;
    if(mold02Time >= mold02Duration) mold02Time = 0;
    if(mold03Time >= mold03Duration) mold03Time = -12;
    if(mold04Time >= mold04Duration) mold04Time = -12;
    if(mold05Time >= mold05Duration) mold05Time = -12;
    if(mold06Time >= mold06Duration) mold06Time = -18;
    if(mold07Time >= mold07Duration) mold07Time = -45;
    if(mold08Time >= mold08Duration && rotaryMechanismTopTime == initRotaryMechanismTopTime + 1) mold08Time = -53;
    if(mold09Time >= mold09Duration) mold09Time = -69;
}

//--------------------------------------------------------------
void ofApp::randomMoldFall(){
    vector<int> rands = {};
    if(mold01Time >= mold01Duration) {
        rands.push_back(0);
    }
    if(mold02Time >= mold02Duration) {
        rands.push_back(1);
    }
    if(mold03Time >= mold03Duration) {
        rands.push_back(2);
    }
    if(mold04Time >= mold04Duration) {
        rands.push_back(3);
    }
    if(mold05Time >= mold05Duration) {
        rands.push_back(4);
    }
    if(mold06Time >= mold06Duration) {
        rands.push_back(5);
    }
    if(mold07Time >= mold07Duration) {
        rands.push_back(6);
    }
    if(mold09Time >= mold09Duration) {
        rands.push_back(7);
    }
    if (rands.size() > 0) {
        int rand = rands[ofRandom(rands.size())];
        switch (rand) {
            case 0:
                if(mold01Time >= mold01Duration) mold01Time = 0;
                break;
            case 1:
                if(mold02Time >= mold02Duration) mold02Time = 0;
                break;
            case 2:
                if(mold03Time >= mold03Duration) mold03Time = 0;
                break;
            case 3:
                if(mold04Time >= mold04Duration) mold04Time = 0;
                break;
            case 4:
                if(mold05Time >= mold05Duration) mold05Time = 0;
                break;
            case 5:
                if(mold06Time >= mold06Duration) mold06Time = 0;
                break;
            case 6:
                if(mold07Time >= mold07Duration) mold07Time = 0;
                break;
            case 7:
                if(mold09Time >= mold09Duration) mold09Time = 0;
                break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'l':
            rotaryMechanismTopTime = 0;
            break;
        case 'r':
            rotaryMechanismTopTime = 0;
            break;
        case 'x':
            ellipse03Move = !ellipse03Move;
            break;
        case 'y':
            rotaryMechanismBottomMove = true;
            break;
        case 'b':
            randomMoldFall();
            break;
        case 'a':
            if (chimneyTime>=chimneyDuration) chimneyTime = 0;
            break;
        case ' ':
            updateParamStop = !updateParamStop;
            break;
        case '0':
            initMold();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case 'y':
            rotaryMechanismBottomMove = false;
            break;
    }
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

//--------------------------------------------------------------
void ofApp::exit(){
    ofxOscMessage m;
    for (size_t i = 0; i < sawNodeIds.size(); ++i) {
        m.setAddress("/n_free");
        m.addIntArg(sawNodeIds[i]);
        scSender.sendMessage(m, false);
        m.clear();
    }
    for (size_t i = 0; i < sineNodeIds.size(); ++i) {
        m.setAddress("/n_free");
        m.addIntArg(sineNodeIds[i]);
        scSender.sendMessage(m, false);
        m.clear();
    }
}
