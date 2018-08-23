#include "ofxVFX.h"

ofxVFX::ofxVFX()
: mMode(ofxVFXMode::NONE), mGlobalColor(1.0), mAttenuation(1.0), mOffsetScale(1.0), mAmt(1.5), mScale(2.0)
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
    mVal1 = ofRandom(1.0);
    mVal2 = ofRandom(1.0);
    mVal3 = ofRandom(1.0);
}

void ofxVFX::begin()
{
    mBaseFbo.begin();
    ofClear(0);
}

void ofxVFX::end()
{
    mBaseFbo.end();
    switch(mMode)
    {
        case ofxVFXMode::NONE:
        {
            mEffectFbo.begin();
            ofClear(0);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::BLOOM:
        {
            // Brightness Threshold Pass
            mBrightnessThreshFbo.begin();
            ofClear(0);
            mBrightnessThreshShader.begin();
            mBrightnessThreshShader.setUniform1f("uBrightnessThresh", 0.3);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mBrightnessThreshShader.end();
            mBrightnessThreshFbo.end();
        
            // Vertical Blur Pass
            mBlurFbo[0].begin();
            ofClear(0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 0);
            mBlurShader.setUniform1f("uOffsetScale", mOffsetScale);
            mBlurShader.setUniform1f("uAttenuation", mAttenuation);
            mBrightnessThreshFbo.draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[0].end();
        
            // Horizontal Blur Pass
            mBlurFbo[1].begin();
            ofClear(0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 1);
            mBlurShader.setUniform1f("uOffsetScale", mOffsetScale);
            mBlurShader.setUniform1f("uAttenuation", mAttenuation);
            mBlurFbo[0].draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[1].end();
        
            // Composite Pass
            mCompositeFbo.begin();
            ofClear(0);
            mCompositeShader.begin();
            mCompositeShader.setUniformTexture("uBlur", mBlurFbo[1].getTexture(), 1);
            mCompositeShader.setUniform1f("uGamma", 2.2);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mCompositeShader.end();
            mCompositeFbo.end();
        
            // Final
            mEffectFbo.begin();
            ofClear(0);
            mCompositeFbo.draw(0, 0, mWidth, mHeight);
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::OPTICALFLOW:
        {
            mFlowVecFbo.begin();
            ofClear(0);
            mFlowShader.begin();
            mFlowShader.setUniformTexture("uBackBuffer", mBackBuffer.getTexture(), 1);
            mFlowShader.setUniform1f("uThresh", 0.01);
            mFlowShader.setUniform1f("uOffset", 1.0);
            mFlowShader.setUniform1f("uLambda", 0.01);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mFlowShader.end();
            mFlowVecFbo.end();
            
            // Vertical Blur Pass
            mBlurFbo[0].begin();
            ofClear(0, 0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 0);
            mBlurShader.setUniform1f("uOffsetScale", mOffsetScale);
            mBlurShader.setUniform1f("uAttenuation", mAttenuation);
            mFlowVecFbo.draw(0, 0, mWidth, mHeight);
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
            ofClear(0);
            mRenderShader.begin();
            mRenderShader.setUniformTexture("uBlur", mBlurFbo[1].getTexture(), 1);
            mRenderShader.setUniform2f("uResolution", mWidth, mHeight);
            mRenderShader.setUniform1f("uAmt", mAmt);
            mRenderShader.setUniform1f("uScale", mScale);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mRenderShader.end();
            mCompositeFbo.end();
            
            // Final
            mEffectFbo.begin();
            ofClear(0);
            mCompositeFbo.draw(0, 0, mWidth, mHeight);
            mEffectFbo.end();
            
            // swap backbuffer
            mBackBuffer.begin();
            ofClear(0);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mBackBuffer.end();
            break;
        }
        case ofxVFXMode::CRT:
        {
            mEffectFbo.begin();
            ofClear(0);
            mCRTShader.begin();
            mCRTShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mCRTShader.end();
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::SOBEL:
        {
            mEffectFbo.begin();
            ofClear(0);
            mSobelShader.begin();
            mSobelShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mSobelShader.end();
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::SYMMETRY:
        {
            mEffectFbo.begin();
            ofClear(0, 0);
            mSymmetryShader.begin();
            mSymmetryShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mSymmetryShader.setUniform2f("uResolution", mWidth, mHeight);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mSymmetryShader.end();
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::STREAK:
        {
            mEffectFbo.begin();
            ofClear(0);
            mStreakShader.begin();
            mStreakShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mStreakShader.setUniform1f("uTime", mTime);
            mStreakShader.setUniform4f("uColor", mGlobalColor);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mStreakShader.end();
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::NOISEWARP:
        {
            mEffectFbo.begin();
            ofClear(0);
            mNoiseWarpShader.begin();
            mNoiseWarpShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mNoiseWarpShader.setUniform1f("uTime", mTime);
            mNoiseWarpShader.setUniform2f("uResolution", mWidth, mHeight);
            mNoiseWarpShader.setUniform1f("uVal1", mVal1);
            mNoiseWarpShader.setUniform1f("uVal2", mVal2);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mNoiseWarpShader.end();
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::CA:
        {
            mEffectFbo.begin();
            ofClear(0);
            mCAShader.begin();
            mCAShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mCAShader.setUniform1f("uTime", mTime);
            mCAShader.setUniform1f("uRandVal", ofRandom(1.0));
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mCAShader.end();
            mEffectFbo.end();
            break;
        }
        case ofxVFXMode::INVERT:
        {
            mEffectFbo.begin();
            ofClear(0);
            mInvertShader.begin();
            mInvertShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mInvertShader.setUniform1f("uTime", mTime);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mInvertShader.end();
            mEffectFbo.end();
            break;
        }
        default:
            break;
    }
}

void ofxVFX::draw()
{
    mEffectFbo.draw(0, 0, mWidth, mHeight);
//    mBlurFbo[1].draw(0, 0, mWidth, mHeight);
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
    
    // Optical Flow
    mBackBuffer.allocate(mWidth, mHeight, GL_RGBA16F);
    mFlowVecFbo.allocate(mWidth, mHeight, GL_RGBA16F);
}

void ofxVFX::setupShaders()
{
    // Bloom
    mBrightnessThreshShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/brightnessThresh.frag", "");
    mBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/blur.frag", "");
    mCompositeShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/composite.frag", "");
    // CRT
    mCRTShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/CRT/CRT.frag", "");
    // Sobel
    mSobelShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/sobel/sobel.frag", "");
    // Symmetry
    mSymmetryShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/symmetry/symmetry.frag", "");
    // Streak
    mStreakShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/streak/streak.frag", "");
    // NoiseWarp
    mNoiseWarpShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/noiseWarp/noiseWarp.frag", "");
    // CA (Chromatic Aberration)
    mCAShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/CA/ca.frag", "");
    // Invert
    mInvertShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/invert/invert.frag", "");
    // Optical Flow
    mFlowShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/opticalFlow/flow.frag", "");
    mRenderShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/opticalFlow/render.frag", "");
}
