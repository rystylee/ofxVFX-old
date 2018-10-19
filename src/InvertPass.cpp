#include "InvertPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

InvertPass::InvertPass()
{}

void InvertPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void InvertPass::update(const float time)
{}

ofFbo& InvertPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0, 255);
    mInvertShader.begin();
    mInvertShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mInvertShader.setUniform2f("uResolution", mWidth, mHeight);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mInvertShader.end();
    mEffectFbo.end();

    return mEffectFbo;
}

void InvertPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void InvertPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void InvertPass::loadShaders()
{
    mInvertShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/invert/invert.frag");
}
