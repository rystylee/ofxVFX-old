#include "ofxVFX.h"

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

ofxVFX::ofxVFX()
: mMode(ofxVFXMode::NONE), mGlobalColor(1.0), mBloomAttenuation(1.0), mBlurScale(1.0)
, mOpticalThresh(0.5), mOpticalScale(5.0), mOpticalFade(0.99), mOpticalForce(0.6), mOpticalAmt(1.0), mIsMNCAReset(0), mCRTDirection(0), mCRTPixelSize(4.0), mSymmetryDirection(0), mIsStreakAdd(0), mIsStreakTwist(1), mStreakScalex(0.01), mStreakScaley(0.0), mStreakSpeed(7.0), mMNCAColorMode(0)
{}

void ofxVFX::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
    mNoiseTex.load("textures/ofxVFX/mnca/noise.png");
}

void ofxVFX::update(const float t)
{
    mTime = t;
    
//    mVal1 = 0;
//    mVal2 = 0;
//    mVal3 = 0;
    
    if(ofRandom(1.0) > 0.6)
    {
        mVal1 = ofRandom(1.0);
        mVal2 = ofRandom(1.0);
        mVal3 = ofRandom(1.0);
    }
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
            mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
            mBlurShader.setUniform1f("uAttenuation", mBloomAttenuation);
            mBrightnessThreshFbo.draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[0].end();
        
            // Horizontal Blur Pass
            mBlurFbo[1].begin();
            ofClear(0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 1);
            mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
            mBlurShader.setUniform1f("uAttenuation", mBloomAttenuation);
            mBlurFbo[0].draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[1].end();
        
            // Composite Pass
            mCompositeFbo.begin();
            ofClear(0);
            mBloomCompositeShader.begin();
            mBloomCompositeShader.setUniformTexture("uBlur", mBlurFbo[1].getTexture(), 1);
            mBloomCompositeShader.setUniform1f("uGamma", 2.2);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mBloomCompositeShader.end();
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
            mFlowPingPong.dst->begin();
            ofClear(0);
            mFlowShader.begin();
            mFlowShader.setUniformTexture("uBackBuffer", mBackBuffer.getTexture(), 1);
            mFlowShader.setUniformTexture("uPrevFlowBuffer", mFlowPingPong.src->getTexture(), 2);
            mFlowShader.setUniform1f("uThresh", mOpticalThresh);
            mFlowShader.setUniform1f("uOffset", mOpticalScale);
            mFlowShader.setUniform1f("uLambda", 0.01);
            mFlowShader.setUniform1f("uForce", mOpticalForce);
            mFlowShader.setUniform1f("uFade", mOpticalFade);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mFlowShader.end();
            mFlowPingPong.dst->end();
            mFlowPingPong.swap();
            
            // Vertical Blur Pass
            mBlurFbo[0].begin();
            ofClear(0, 0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 0);
            mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
            mBlurShader.setUniform1f("uAttenuation", mBloomAttenuation);
            mFlowPingPong.dst->draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[0].end();
            
            // Horizontal Blur Pass
            mBlurFbo[1].begin();
            ofClear(0, 0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 1);
            mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
            mBlurShader.setUniform1f("uAttenuation", mBloomAttenuation);
            mBlurFbo[0].draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[1].end();
            
            // Composite Pass
            mCompositeFbo.begin();
            ofClear(0);
            mFlowRenderShader.begin();
            mFlowRenderShader.setUniformTexture("uFlow", mBlurFbo[1].getTexture(), 1);
            mFlowRenderShader.setUniform2f("uResolution", mWidth, mHeight);
            mFlowRenderShader.setUniform1f("uAmt", mOpticalAmt);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mFlowRenderShader.end();
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
            mCRTShader.setUniform1i("uDirection", mCRTDirection);
            mCRTShader.setUniform1f("uPixelSize", mCRTPixelSize);
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
            mSymmetryShader.setUniform1i("uDirection", mSymmetryDirection);
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
            mStreakShader.setUniform1i("uIsAdd", mIsStreakAdd);
            mStreakShader.setUniform1i("uIsTwist", mIsStreakTwist);
            mStreakShader.setUniform1f("uScalex", mStreakScalex);
            mStreakShader.setUniform1f("uScaley", mStreakScaley);
            mStreakShader.setUniform1f("uSpeed", mStreakSpeed);
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
        case ofxVFXMode::MNCA:
        {
            if(ofRandom(1.0) < 0.2)
            {
                mMNCAPingPong.dst->begin();
                ofClear(0);
                ofPushStyle();
                ofSetColor(255);
                mBaseFbo.draw(0, 0, mWidth, mHeight);
                mMNCAPingPong.dst->end();
                mMNCAPingPong.swap();
                ofPopStyle();
            }
            
            mMNCAPingPong.dst->begin();
            ofClear(0);
            mMNCA0Shader.begin();
            mMNCA0Shader.setUniformTexture("uPrevBuffer", mMNCAPingPong.src->getTexture(), 0);
            mMNCA0Shader.setUniformTexture("uNoiseTex", mNoiseTex.getTexture(), 1);
            mMNCA0Shader.setUniform1i("uIsReset", mIsMNCAReset);
            mMNCAPingPong.src->draw(0, 0, mWidth, mHeight);
            mMNCA0Shader.end();
            mMNCAPingPong.dst->end();
            mMNCAPingPong.swap();
            
            mEffectFbo.begin();
            ofClear(0);
            mMNCARenderShader.begin();
            mMNCARenderShader.setUniformTexture("uSimuTex", mMNCAPingPong.dst->getTexture(), 0);
            mMNCARenderShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 1);
            mMNCARenderShader.setUniform2f("uResolution", mWidth, mHeight);
            mMNCARenderShader.setUniform1i("uColorMode", mMNCAColorMode);
            mMNCARenderShader.setUniform1f("uTime", mTime);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mMNCARenderShader.end();
            mEffectFbo.end();
            
            mIsMNCAReset++;
            break;
        }
        case ofxVFXMode::INK:
            mInkPingPong.dst->begin();
            ofClear(0);
            mInkShader.begin();
            mInkShader.setUniformTexture("uPrevBuffer", mInkPingPong.src->getTexture(), 0);
            mInkShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 1);
            mInkShader.setUniform1f("uTime", mTime);
            mInkShader.setUniform2f("uResolution", mWidth, mHeight);
            mInkShader.setUniform1i("uCount", ofGetFrameNum());
            mInkPingPong.src->draw(0, 0, mWidth, mHeight);
            mInkShader.end();
            mInkPingPong.dst->end();
            mInkPingPong.swap();
            
            mEffectFbo.begin();
            ofClear(0);
            mInkRenderShader.begin();
            mInkRenderShader.setUniformTexture("uResult", mInkPingPong.dst->getTexture(), 0);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mInkRenderShader.end();
            mEffectFbo.end();
            break;
        case ofxVFXMode::ZOOMBLUR:
            mEffectFbo.begin();
            ofClear(0);
            mZoomBlurShader.begin();
            mZoomBlurShader.setUniformTexture("uBase", mBaseFbo.getTexture(), 0);
            mZoomBlurShader.setUniform2f("uResolution", mWidth, mHeight);
            mZoomBlurShader.setUniform1f("uVal1", mVal1);
            ofDrawRectangle(0, 0, mWidth, mHeight);
            mZoomBlurShader.end();
            mEffectFbo.end();
            break;
        default:
            break;
    }
}

void ofxVFX::draw(const float width, const float height)
{
    mEffectFbo.draw(0, 0, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void ofxVFX::initBuffers()
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
    auto opticalColor = make_unique<float[]>(mWidth * mHeight * 4);
    for(int x=0; x<mWidth; x++)
    {
        for(int y=0; y<mHeight; y++)
        {
//            const int i = y * mWidth + x;
            const int i = x * mHeight + y;
            opticalColor[i * 4 + 0] = 0.0;
            opticalColor[i * 4 + 1] = 0.0;
            opticalColor[i * 4 + 2] = 0.0;
            opticalColor[i * 4 + 3] = 0.0;
        }
    }
    mFlowPingPong.allocate(mWidth, mHeight, GL_RGB32F);
    mFlowPingPong.src->getTexture().loadData(opticalColor.get(), mWidth, mHeight, GL_RGB32F);
    mFlowPingPong.dst->getTexture().loadData(opticalColor.get(), mWidth, mHeight, GL_RGB32F);
    
    // MNCA
    auto mncaColor = make_unique<float[]>(mWidth * mHeight * 3);
    for(int x=0; x<mWidth; x++)
    {
        for(int y=0; y<mHeight; y++)
        {
            const int i = x * mHeight + y;
            mncaColor[i * 3 + 0] = ofRandom(1.0);
            mncaColor[i * 3 + 1] = ofRandom(1.0);
            mncaColor[i * 3 + 2] = ofRandom(1.0);
        }
    }
    mMNCAPingPong.allocate(mWidth, mHeight, GL_RGB32F);
    mMNCAPingPong.src->getTexture().loadData(mncaColor.get(), mWidth, mHeight, GL_RGB32F);
    mMNCAPingPong.dst->getTexture().loadData(mncaColor.get(), mWidth, mHeight, GL_RGB32F);
    
    // Ink
    auto inkColor = make_unique<float[]>(mWidth * mHeight * 4);
    for(int x=0; x<mWidth; x++)
    {
        for(int y=0; y<mHeight; y++)
        {
            const int i = x * mHeight + y;
            inkColor[i * 4 + 0] = 0.0;
            inkColor[i * 4 + 1] = 0.0;
            inkColor[i * 4 + 2] = 0.0;
            inkColor[i * 4 + 3] = 0.0;
        }
    }
    mInkPingPong.allocate(mWidth, mHeight, GL_RGBA);
    mInkPingPong.src->getTexture().loadData(inkColor.get(), mWidth, mHeight, GL_RGBA);
    mInkPingPong.dst->getTexture().loadData(inkColor.get(), mWidth, mHeight, GL_RGBA);
}

void ofxVFX::loadShaders()
{
    // Bloom
    mBrightnessThreshShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/brightnessThresh.frag");
    mBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/blur.frag");
    mBloomCompositeShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/composite.frag");
    // CRT
    mCRTShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/CRT/CRT.frag");
    // Sobel
    mSobelShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/sobel/sobel.frag");
    // Symmetry
    mSymmetryShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/symmetry/symmetry.frag");
    // Streak
    mStreakShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/streak/streak.frag");
    // NoiseWarp
    mNoiseWarpShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/noiseWarp/noiseWarp.frag");
    // CA (Chromatic Aberration)
    mCAShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/CA/ca.frag");
    // Invert
    mInvertShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/invert/invert.frag");
    // Optical Flow
    mFlowShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/opticalFlow/flow.frag");
    mFlowRenderShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/opticalFlow/render.frag");
    // MNCA (Multiple Neighborhoods Cellular Automata)
    mMNCA0Shader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/MNCA/mnca0.frag");
    mMNCARenderShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/MNCA/render.frag");
    // Ink
    mInkShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/ink/ink.frag");
    mInkRenderShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/ink/render.frag");
    // Zoom Blur
    mZoomBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/zoomBlur/zoomBlur.frag");
}
