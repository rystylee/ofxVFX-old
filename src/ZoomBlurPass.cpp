#include "ZoomBlurPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

ZoomBlurPass::ZoomBlurPass()
{}

void ZoomBlurPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void ZoomBlurPass::update(const float time)
{
    mTime = time;
    
    if(ofRandom(1.0) > 0.6)
    {
        mVal1 = ofRandom(1.0);
    }
}

ofFbo& ZoomBlurPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0, 255);
    mZoomBlurShader.begin();
    mZoomBlurShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mZoomBlurShader.setUniform2f("uResolution", mWidth, mHeight);
    mZoomBlurShader.setUniform1f("uVal1", mVal1);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mZoomBlurShader.end();
    mEffectFbo.end();

    return mEffectFbo;
}

void ZoomBlurPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void ZoomBlurPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void ZoomBlurPass::loadShaders()
{
    mZoomBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/zoomBlur/zoomBlur.frag");
}
