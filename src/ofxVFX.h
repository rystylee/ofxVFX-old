#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"

enum class ofxVFXMode
{
    NONE,
    BLOOM,
    CRT,
    SOBEL,
    SYMMETRY,
    STREAK,
    NOISEWARP,
};

class ofxVFX
{
public:
    ofxVFX();
    void setup(const int w, const int h);
    void update(const float t);
    void bang();
    void begin();
    void end();
    void draw();

    // Setter
    inline void setVFXMode(const ofxVFXMode mode) { mMode = mode; }
    inline void setBloomAttenuation(const float attenuation) { mAttenuation = attenuation; }
    inline void setBloomOffsetScale(const float offsetScale) { mOffsetScale = offsetScale; }
    
private:
    void setupFbos();
    void setupShaders();
    
    ofxVFXMode mMode;
    
    int mWidth, mHeight;
    ofFbo mBaseFbo, mEffectFbo;
    
    float mTime;
    float mVal1, mVal2, mVal3;
    
    // Bloom
    ofFbo mBrightnessThreshFbo, mBlurFbo[2], mCompositeFbo;
    ofxAutoReloadedShader mBrightnessThreshShader, mBlurShader, mCompositeShader;
    float mAttenuation, mOffsetScale;
    
    // CRT
    ofxAutoReloadedShader mCRTShader;
    
    // Sobel
    ofxAutoReloadedShader mSobelShader;
    
    // Symmetry
    ofxAutoReloadedShader mSymmetryShader;
    
    // Streak
    ofxAutoReloadedShader mStreakShader;
    
    // NoiseWarp
    ofxAutoReloadedShader mNoiseWarpShader;
};
