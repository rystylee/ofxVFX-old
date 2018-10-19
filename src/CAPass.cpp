#include "CAPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

CAPass::CAPass()
{}

void CAPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void CAPass::update(const float time)
{
    mTime = time;
}

ofFbo& CAPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0, 255);
    mCAShader.begin();
    mCAShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mCAShader.setUniform2f("uResolution", mWidth, mHeight);
    mCAShader.setUniform1f("uTime", mTime);
    mCAShader.setUniform1f("uRandVal", ofRandom(1.0));
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mCAShader.end();
    mEffectFbo.end();
    
    return mEffectFbo;
}

void CAPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void CAPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void CAPass::loadShaders()
{
    mCAShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/ca/ca.frag");
}
