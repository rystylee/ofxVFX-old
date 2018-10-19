#include "EdgePass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

EdgePass::EdgePass()
{}

void EdgePass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void EdgePass::update(const float time)
{}

ofFbo& EdgePass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0, 255);
    mEdgeShader.begin();
    mEdgeShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mEdgeShader.setUniform2f("uResolution", mWidth, mHeight);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mEdgeShader.end();
    mEffectFbo.end();
    
    return mEffectFbo;
}

void EdgePass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void EdgePass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void EdgePass::loadShaders()
{
    mEdgeShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/edge/edge.frag");
}
