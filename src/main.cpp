#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;

    settings.setSize(1280, 1024);
    settings.decorated = false;
    settings.windowMode = OF_WINDOW;

    ofCreateWindow(settings);
    ofRunApp(new ofApp());

}
