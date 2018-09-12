# ofxVFX[WIP]

## Description

ofxVFX is an openframeworks addon for visual effects.

## Environment

* openFrameworks 0.10.0

## Usage

* Copy the `shaders/ofxVFX/` directory to your `bin/data/` directory.  

* Change your main.cpp to use programmable shader.  

``````````
int main()
{
    ofGLWindowSettings s;
    s.setGLVersion(4, 1); // or the version you use
    s.setSize(1920, 1080);
    ofCreateWindow(s);
    
    ofRunApp(new ofApp());
}
``````````

* Declare an instance of `ofxVFX`

``````````
ofxVFX mVFX;
``````````

* In your `setup()` function setup the `ofxVFX` class width screen resolution and then set the effect mode.

``````````
mVFX.setup(mWidth, mHeight);
mVFX.setVFXMode(ofxVFXMode::BLOOM);
``````````

* You have to call `mVFX.update(time)` function in rendering loop to update effects.

``````````
mVFX.update(ofGetElapsedTimef());
``````````

* Render your scene between `mVFX.begin()` amd `mVFX.end()`.

``````````
mVFX.begin();

// some codes to render scene

mVFX.end();
``````````

* Call `mVFX.draw(width, height)` in your `draw()` function.

``````````
void draw()
{
    mVFX.draw(width, height);
}
``````````

* You can change effect parameters like below. Check the example for more info!

``````````
mVFX.setBlurScale(2.0);
``````````
