#include "ofxVFX.h"

ofxVFX::ofxVFX()
: mMode(ofxVFXMode::NONE), mAttenuation(1.0), mOffsetScale(1.0)
{}

void ofxVFX::setup(const int w, const int h)
{
    mWidth = w; mHeight = h;
    setupFbos();
    setupShaders();
}

void ofxVFX::update(const float t)
{
    mTime = t;
    
    mVal1 = 0;
    mVal2 = 0;
    mVal3 = 0;
}

void ofxVFX::bang()
{
    mVal1 = ofRandom(100);
    mVal2 = ofRandom(100);
    mVal3 = ofRandom(100);
}

void ofxVFX::begin()
{
    mBaseFbo.begin();
    ofClear(0, 255);
}

void ofxVFX::end()
{
    mBaseFbo.end();
    
    switch(mMode)
    {
        case ofxVFXMode::NONE:
            mEffectFbo.begin();
            ofSetColor(255);
            ofClear(0, 0);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mEffectFbo.end();
            break;
        case ofxVFXMode::BLOOM:
            // Brightness Threshold Pass
            mBrightnessThreshFbo.begin();
            ofClear(0, 0);
            mBrightnessThreshShader.begin();
            mBrightnessThreshShader.setUniform1f("uBrightnessThresh", 0.3);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mBrightnessThreshShader.end();
            mBrightnessThreshFbo.end();
        
            // Vertical Blur Pass
            mBlurFbo[0].begin();
            ofClear(0, 0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 0);
            mBlurShader.setUniform1f("uOffsetScale", mOffsetScale);
            mBlurShader.setUniform1f("uAttenuation", mAttenuation);
            mBrightnessThreshFbo.draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[0].end();
        
            // Horizontal Blur Pass
            mBlurFbo[1].begin();
            ofClear(0, 0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 1);
            mBlurShader.setUniform1f("uOffsetScale", mOffsetScale);
            mBlurShader.setUniform1f("uAttenuation", mAttenuation);
            mBlurFbo[0].draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[1].end();
        
            // Composite Pass
            mCompositeFbo.begin();
            ofClear(0, 0);
            mCompositeShader.begin();
            mCompositeShader.setUniformTexture("uBlur", mBlurFbo[1].getTexture(), 1);
            mCompositeShader.setUniform1f("uGamma", 2.2);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mCompositeShader.end();
            mCompositeFbo.end();
        
            // Final
            mEffectFbo.begin();
            ofSetColor(255);
            ofClear(0, 0);
            mCompositeFbo.draw(0, 0, mWidth, mHeight);
            mEffectFbo.end();
            break;
        case ofxVFXMode::CRT:
            mEffectFbo.begin();
            ofClear(0, 0);
            mCRTShader.begin();
            mCRTShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mCRTShader.end();
            mEffectFbo.end();
            break;
        case ofxVFXMode::SOBEL:
            mEffectFbo.begin();
            ofClear(0, 0);
            mSobelShader.begin();
            mSobelShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mSobelShader.end();
            mEffectFbo.end();
            break;
        case ofxVFXMode::SYMMETRY:
            mEffectFbo.begin();
            ofClear(0, 0);
            mSymmetryShader.begin();
            mSymmetryShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mSymmetryShader.setUniform2f("uResolution", mWidth, mHeight);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mSymmetryShader.end();
            mEffectFbo.end();
            break;
        case ofxVFXMode::STREAK:
            mEffectFbo.begin();
            ofClear(0, 0);
            mStreakShader.begin();
            mStreakShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mStreakShader.setUniform1f("uTime", mTime);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mStreakShader.end();
            mEffectFbo.end();
            break;
        case ofxVFXMode::TWIST:
            mEffectFbo.begin();
            ofClear(0, 0);
            mTwistShader.begin();
            mTwistShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mTwistShader.setUniform1f("uTime", mTime);
            mTwistShader.setUniform1f("uRand", ofRandom(1));
            mTwistShader.setUniform1f("uVal1", mVal1);
            mTwistShader.setUniform1f("uVal2", mVal2);
            mTwistShader.setUniform1f("uVal3", mVal3);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mTwistShader.end();
            mEffectFbo.end();
            break;
        default:
            break;
    }
}

void ofxVFX::draw()
{
    mEffectFbo.draw(0, 0, mWidth, mHeight);
}

void ofxVFX::setupFbos()
{
    mBaseFbo.allocate(mWidth, mHeight, GL_RGBA16F);
//    mBaseFbo.getTexture().getTextureData().bFlipTexture = true;
    mEffectFbo.allocate(mWidth, mHeight, GL_RGBA16F);
    
    // Bloom
    mBrightnessThreshFbo.allocate(mWidth, mHeight, GL_RGBA16F);
    for(int i=0; i<2; i++)
    {
        mBlurFbo[i].allocate(mWidth, mHeight, GL_RGBA16F);
    }
    mCompositeFbo.allocate(mWidth, mHeight, GL_RGBA16F);
}

void ofxVFX::setupShaders()
{
    // Bloom
    mBrightnessThreshShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Bloom/brightnessThresh.frag", "");
    mBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Bloom/blur.frag", "");
    mCompositeShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Bloom/composite.frag", "");
    // CRT
    mCRTShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/CRT/CRT.frag", "");
    // Sobel
    mSobelShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Sobel/sobel.frag", "");
    // Symmetry
    mSymmetryShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Symmetry/symmetry.frag", "");
    // Streak
    mStreakShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Streak/streak.frag", "");
    // Twist
    mTwistShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Twist/Twist.frag", "");
}
