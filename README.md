# ofxVFX[WIP]

## How to use

* copy the `shaders/ofxVFX/` directory to your `bin/data/` directory.  

* change your main.cpp  


``````````
int main()
{
    ofGLWindowSettings s;
    s.setGLVersion(4, 0); // or the version you use
    s.width = 1280
    s.height = 720
    ofCreateWindow(s);
    
    ofRunApp(new ofApp());
}
``````````
