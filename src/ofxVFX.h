#pragma once

#include "ofMain.h"
#include "PingPongBuffer.hpp"
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
    inline void setBloomAttenuation(const float attenuation) { mBloomAttenuation = attenuation; }
    inline void setBlurScale(const float blurScale) { mBlurScale = blurScale; }
    inline void setOpticalThresh(const float opticalThresh) { mOpticalThresh = opticalThresh; }
    inline void setOpticalScale(const float opticalScale) { mOpticalScale = opticalScale; }
    inline void setOpticalFade(const float opticalFade) { mOpticalFade = opticalFade; }
    inline void setOpticalForce(const float opticalForce) { mOpticalForce = opticalForce; }
    inline void setOpticalAmt(const float opticalAmt) { mOpticalAmt = opticalAmt; }
    
    static const int NUM_VFX_MODES = static_cast<int>(ofxVFXMode::MAX);
    
private:
    void initFbos();
    void loadShaders();

    ofxVFXMode mMode;

    int mWidth, mHeight;
    ofFbo mBaseFbo, mEffectFbo;
    
    float mTime;
    float mVal1, mVal2, mVal3;
    ofFloatColor mGlobalColor;
    
    // Bloom
    ofFbo mBrightnessThreshFbo, mBlurFbo[2], mCompositeFbo;
    ofxAutoReloadedShader mBrightnessThreshShader, mBlurShader, mCompositeShader; // share with Optical flow
    float mBloomAttenuation, mBlurScale;
    
    // Optical Flow
    ofFbo mBackBuffer; // and used the mBlurFbo[2]
    PingPongBuffer mFlowPingPong;
    ofxAutoReloadedShader mFlowShader, mRenderShader; // and used the mBlurShader
    float mOpticalThresh, mOpticalScale, mOpticalFade, mOpticalForce, mOpticalAmt;
    
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
