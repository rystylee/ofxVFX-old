#include "OpticalFlowPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

OpticalFlowPass::OpticalFlowPass()
: mBlurScale(1.0), mOpticalThresh(0.5), mOpticalScale(5.0), mOpticalFade(0.99), mOpticalForce(0.6), mOpticalAmt(1.0)
{}

void OpticalFlowPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void OpticalFlowPass::update(const float time)
{}

ofFbo& OpticalFlowPass::process(ofFbo& baseFbo)
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
    baseFbo.draw(0, 0, mWidth, mHeight);
    mFlowShader.end();
    mFlowPingPong.dst->end();
    mFlowPingPong.swap();
    
    // Vertical Blur Pass
    mBlurFbo[0].begin();
    ofClear(0, 0);
    mBlurShader.begin();
    mBlurShader.setUniform1i("uDirection", 0);
    mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
    mFlowPingPong.dst->draw(0, 0, mWidth, mHeight);
    mBlurShader.end();
    mBlurFbo[0].end();
    
    // Horizontal Blur Pass
    mBlurFbo[1].begin();
    ofClear(0, 0);
    mBlurShader.begin();
    mBlurShader.setUniform1i("uDirection", 1);
    mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
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
    baseFbo.draw(0, 0, mWidth, mHeight);
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
    baseFbo.draw(0, 0, mWidth, mHeight);
    mBackBuffer.end();

    return mEffectFbo;
}

void OpticalFlowPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void OpticalFlowPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
    
    mBlurFbo[0].allocate(mWidth, mHeight);
    mBlurFbo[1].allocate(mWidth, mHeight);
    mCompositeFbo.allocate(mWidth, mHeight);
    
    mBackBuffer.allocate(mWidth, mHeight, GL_RGBA);
    auto opticalColor = std::make_unique<float[]>(mWidth * mHeight * 3);
    for(int x=0; x<mWidth; x++)
    {
        for(int y=0; y<mHeight; y++)
        {
            const int i = x * mHeight + y;
            opticalColor[i * 3 + 0] = 0.0;
            opticalColor[i * 3 + 1] = 0.0;
            opticalColor[i * 3 + 2] = 0.0;
        }
    }
    mFlowPingPong.allocate(mWidth, mHeight, GL_RGB32F);
    mFlowPingPong.src->getTexture().loadData(opticalColor.get(), mWidth, mHeight, GL_RGB32F);
    mFlowPingPong.dst->getTexture().loadData(opticalColor.get(), mWidth, mHeight, GL_RGB32F);
}

void OpticalFlowPass::loadShaders()
{
    mBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/opticalFlow/blur.frag");
    mFlowShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/opticalFlow/flow.frag");
    mFlowRenderShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/opticalFlow/render.frag");
}

