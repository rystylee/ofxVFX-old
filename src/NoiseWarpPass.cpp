#include "NoiseWarpPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

NoiseWarpPass::NoiseWarpPass()
{}

void NoiseWarpPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void NoiseWarpPass::update(const float time)
{
    mTime = time;
    
    if(ofRandom(1.0) > 0.6)
    {
        mVal1 = ofRandom(1.0);
        mVal2 = ofRandom(1.0);
    }
}

ofFbo& NoiseWarpPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0);
    mNoiseWarpShader.begin();
    mNoiseWarpShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mNoiseWarpShader.setUniform1f("uTime", mTime);
    mNoiseWarpShader.setUniform2f("uResolution", mWidth, mHeight);
    mNoiseWarpShader.setUniform1f("uVal1", mVal1);
    mNoiseWarpShader.setUniform1f("uVal2", mVal2);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mNoiseWarpShader.end();
    mEffectFbo.end();

    return mEffectFbo;
}

void NoiseWarpPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void NoiseWarpPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void NoiseWarpPass::loadShaders()
{
    mNoiseWarpShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/noiseWarp/noiseWarp.frag");
}
