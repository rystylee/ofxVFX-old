#pragma once

#include "ofMain.h"
#include "PingPongBuffer.hpp"

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
    MNCA, // (Multiple Neighborhoods Cellular Automata)
    INK,
    ZOOMBLUR,
    MAX,
};

class ofxVFX
{
public:
    ofxVFX();
    void setup(const int width, const int height);
    void update(const float t);
    void bang();
    void begin();
    void end();
    void draw(const float width, const float height);

    // Setter
    inline void setGlobalColor(const ofFloatColor& color) { mGlobalColor = color; }
    inline void setVFXMode(const ofxVFXMode mode) { mMode = mode; }
    // Blur
    inline void setBlurScale(const float blurScale) { mBlurScale = blurScale; }
    // Bloom
    inline void setBloomAttenuation(const float attenuation) { mBloomAttenuation = attenuation; }
    // Optical Flow
    inline void setOpticalThresh(const float opticalThresh) { mOpticalThresh = opticalThresh; }
    inline void setOpticalScale(const float opticalScale) { mOpticalScale = opticalScale; }
    inline void setOpticalFade(const float opticalFade) { mOpticalFade = opticalFade; }
    inline void setOpticalForce(const float opticalForce) { mOpticalForce = opticalForce; }
    inline void setOpticalAmt(const float opticalAmt) { mOpticalAmt = opticalAmt; }
    // CRT
    inline void setCRTDirection(const int CRTDirection) { mCRTDirection = CRTDirection; }
    inline void setCRTPixelSize(const float CRTPixelSize) { mCRTPixelSize = CRTPixelSize; }
    // Symmetry
    inline void setSymmetryDirection(const int symmetryDirection) { mSymmetryDirection = symmetryDirection; }
    // Streak
    inline void setIsStreakAdd(const int isStreakAdd) { mIsStreakAdd = isStreakAdd; }
    inline void setIsStreakTwist(const int isStreakTwist) { mIsStreakTwist = isStreakTwist; }
    inline void setStreakScalex(const float streakScalex) { mStreakScalex = streakScalex; }
    inline void setStreakScaley(const float streakScaley) { mStreakScaley = streakScaley; }
    inline void setStreakSpeed(const float streakSpeed) { mStreakSpeed = streakSpeed; }
    // MNCA
	inline void setMNCAColorMode(const int colorMode) { mMNCAColorMode = colorMode;  }
    
    // Getter
    inline const int getNumVFXMode() { return static_cast<int>(ofxVFXMode::MAX); }
    
private:
    void initBuffers();
    void loadShaders();

    ofxVFXMode mMode;

    int mWidth, mHeight;
    ofFbo mBaseFbo, mEffectFbo;
    
    float mTime;
    float mVal1, mVal2, mVal3;
    ofFloatColor mGlobalColor;
    
    // Bloom
    ofFbo mBrightnessThreshFbo, mBlurFbo[2], mCompositeFbo;
    ofShader mBrightnessThreshShader, mBlurShader, mBloomCompositeShader; // share with Optical flow
    float mBloomAttenuation, mBlurScale;
    
    // Optical Flow
    ofFbo mBackBuffer; // and use the mBlurFbo[2]
    PingPongBuffer mFlowPingPong;
    ofShader mFlowShader, mFlowRenderShader; // and used the mBlurShader
    float mOpticalThresh, mOpticalScale, mOpticalFade, mOpticalForce, mOpticalAmt;
    
    // CRT
    ofShader mCRTShader;
    int mCRTDirection;
    float mCRTPixelSize;
    
    // Sobel
    ofShader mSobelShader;
    
    // Symmetry
    ofShader mSymmetryShader;
    int mSymmetryDirection;
    
    // Streak
    ofShader mStreakShader;
    int mIsStreakAdd;
    int mIsStreakTwist;
    float mStreakScalex;
    float mStreakScaley;
    float mStreakSpeed;
    
    // NoiseWarp
    ofShader mNoiseWarpShader;
    
    // CA (Chromatic Aberration)
    ofShader mCAShader;
    
    // Invert
    ofShader mInvertShader;
    
    // MNCA (Multiple Neighborhoods Cellular Automata)
    ofShader mMNCA0Shader;
    ofShader mMNCARenderShader;
    ofShader mMNCACompositeShader;
    PingPongBuffer mMNCAPingPong;
    int mIsMNCAReset;
	int mMNCAColorMode;
    ofImage mNoiseTex;
    
    // Ink
    PingPongBuffer mInkPingPong;
    ofShader mInkShader;
    ofShader mInkRenderShader;
    
    // Zoom Blur
    ofShader mZoomBlurShader;
};
