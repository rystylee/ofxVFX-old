#include "Symmetry.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

SymmetryPass::SymmetryPass()
: mSymmetryDirection(0)
{}

void SymmetryPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void SymmetryPass::update(const float time)
{}

ofFbo& SymmetryPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0, 0);
    mSymmetryShader.begin();
    mSymmetryShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mSymmetryShader.setUniform2f("uResolution", mWidth, mHeight);
    mSymmetryShader.setUniform1i("uDirection", mSymmetryDirection);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mSymmetryShader.end();
    mEffectFbo.end();

    return mEffectFbo;
}

void SymmetryPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void SymmetryPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void SymmetryPass::loadShaders()
{
    mSymmetryShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/symmetry/symmetry.frag");
}
