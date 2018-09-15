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

* In your `setup()` function, call `mVFX.addPass<ofxVFX::RenderPass>()` to add the effect pass and then call `mVFX.generate(width, height)` to setup each effect.

``````````
mVFX.addPass<ofxVFX::BloomPass>();
mVFX.addPass<ofxVFX::OpticalFlowPass>();
mVFX.addPass<ofxVFX::CRTPass>();
mVFX.addPass<ofxVFX::EdgePass>();
mVFX.addPass<ofxVFX::SymmetryPass>();
mVFX.addPass<ofxVFX::StreakPass>();
mVFX.addPass<ofxVFX::NoiseWarpPass>();
mVFX.addPass<ofxVFX::CAPass>();
mVFX.addPass<ofxVFX::InvertPass>();
mVFX.addPass<ofxVFX::MNCAPass>();
mVFX.addPass<ofxVFX::InkPass>();
mVFX.addPass<ofxVFX::ZoomBlurPass>();

mVFX.generate(width, height);
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

* Call `mVFX.draw(0, 0, width, height)` in your `draw()` function.

``````````
void draw()
{
    mVFX.draw(0, 0, width, height);
}
``````````

* You can change effect parameters like below. Check the example for more info!

``````````
// Bloom
mVFX.getRenderPassPtr(int renderPassIndex)->setBloomBlurScale(mBlurScale);
``````````

## Reference

* [neilmendoza/ofxPostProcessing](https://github.com/neilmendoza/ofxPostProcessing)

* [nama-gatsuo/ofxDeferredShading](https://github.com/nama-gatsuo/ofxDeferredShading)