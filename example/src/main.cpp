#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
    ofGLFWWindowSettings s;
    s.setGLVersion(4, 1);
    s.setSize(1920, 1080);
    ofCreateWindow(s);
    
    ofRunApp(new ofApp());
}
