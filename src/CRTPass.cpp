#include "CRTPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

CRTPass::CRTPass()
: mCRTDirection(0), mCRTPixelSize(4.0)
{}

void CRTPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void CRTPass::update(const float time)
{}

ofFbo& CRTPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0);
    mCRTShader.begin();
    mCRTShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mCRTShader.setUniform1i("uDirection", mCRTDirection);
    mCRTShader.setUniform1f("uPixelSize", mCRTPixelSize);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mCRTShader.end();
    mEffectFbo.end();
    
    return mEffectFbo;
}

void CRTPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void CRTPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void CRTPass::loadShaders()
{
    mCRTShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/CRT/CRT.frag");
}
