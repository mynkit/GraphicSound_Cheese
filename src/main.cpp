#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;

    settings.setSize(1024, 1024);
    settings.decorated = false;
    settings.windowMode = OF_FULLSCREEN;

    ofCreateWindow(settings);
    ofRunApp(new ofApp());

}
