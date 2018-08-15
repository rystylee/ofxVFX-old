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
    inline void setBloomAttenuation(const float attenuation) { mAttenuation = attenuation; }
    inline void setBloomOffsetScale(const float offsetScale) { mOffsetScale = offsetScale; }
    
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
    float mAttenuation, mOffsetScale;
};
