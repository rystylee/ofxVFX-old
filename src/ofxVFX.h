#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"

enum class ofxVFXMode
{
    NONE,
    BLOOM,
    OPTICALFLOW,
    CRT,
    SOBEL,
    SYMMETRY,
    STREAK,
    NOISEWARP,
    CA, // (Chromatic Aberration)
    INVERT,
    MAX,
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
    inline void setGlobalColor(const ofFloatColor& color) { mGlobalColor = color; }
    inline void setVFXMode(const ofxVFXMode mode) { mMode = mode; }
    inline void setAttenuation(const float attenuation) { mAttenuation = attenuation; }
    inline void setOffsetScale(const float offsetScale) { mOffsetScale = offsetScale; }
    inline void setOpticalAmt(const float amt) { mAmt = amt; }
    inline void setOpticalScale(const float scale) { mScale = scale; }

    static const int NUM_VFX_MODES = static_cast<int>(ofxVFXMode::MAX);
    
private:
    void setupFbos();
    void setupShaders();

    ofxVFXMode mMode;

    int mWidth, mHeight;
    ofFbo mBaseFbo, mEffectFbo;
    
    float mTime;
    float mVal1, mVal2, mVal3;
    ofFloatColor mGlobalColor;
    
    // Bloom
    ofFbo mBrightnessThreshFbo, mBlurFbo[2], mCompositeFbo;
    ofxAutoReloadedShader mBrightnessThreshShader, mBlurShader, mCompositeShader; // share with Optical flow
    float mAttenuation, mOffsetScale;
    
    // Optical Flow
    ofFbo mBackBuffer, mFlowVecFbo; // and used the mBlurFbo[2]
    ofxAutoReloadedShader mFlowShader, mRenderShader; // and used the mBlurShader
    float mAmt, mScale;
    
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
    
    // CA (Chromatic Aberration)
    ofxAutoReloadedShader mCAShader;
    
    // Invert
    ofxAutoReloadedShader mInvertShader;
};
