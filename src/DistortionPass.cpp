#include "DistortionPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

DistortionPass::DistortionPass()
{}

void DistortionPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void DistortionPass::update(const float time)
{
    mTime = time;
}

ofFbo& DistortionPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0, 255);
    mDistortionShader.begin();
    mDistortionShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mDistortionShader.setUniform2f("uResolution", mWidth, mHeight);
    mDistortionShader.setUniform1f("uTime", mTime);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mDistortionShader.end();
    mEffectFbo.end();
    
    return mEffectFbo;
}

void DistortionPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void DistortionPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void DistortionPass::loadShaders()
{
    mDistortionShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/distortion/distortion.frag");
}
