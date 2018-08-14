#pragma once

#include "ofMain.h"

enum class ofxVFXMode
{
    NONE,
    SOBEL,
    BLOOM,
};

class ofxVFX
{
public:
    ofxVFX();
    void setup(const int w, const int h);
    void update(const float t);
    void begin();
    void end();
    void draw();
    
    // Setter
    inline void setVFXMode(const ofxVFXMode mode) { mMode = mode; }
    
private:
    void setupFbos();
    void setupShaders();
    
    ofxVFXMode mMode;
    
    int mWidth, mHeight;
    ofFbo mBaseFbo, mEffectFbo;
    
    // Sobel
    ofShader mSobelShader;
    
    // Bloom
    ofFbo mBrightnessThreshFbo, mBlurFbo[2], mCompositeFbo;
    ofShader mBrightnessThreshShader, mBlurShader, mCompositeShader;
};
