#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;

    settings.setSize(1000, 1000);
    settings.decorated = false;
    settings.windowMode = OF_WINDOW;

    ofCreateWindow(settings);
    ofRunApp(new ofApp());

}
