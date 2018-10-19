#include "StreakPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

StreakPass::StreakPass()
: mColor(1.0), mIsStreakAdd(true), mIsStreakTwist(false), mStreakScalex(0.01), mStreakScaley(0.0), mStreakSpeed(7.0)
{}

void StreakPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void StreakPass::update(const float time)
{
    mTime = time;
}

ofFbo& StreakPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0, 255);
    mStreakShader.begin();
    mStreakShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mStreakShader.setUniform2f("uResolution", mWidth, mHeight);
    mStreakShader.setUniform1f("uTime", mTime);
    mStreakShader.setUniform1i("uIsAdd", static_cast<int>(mIsStreakAdd));
    mStreakShader.setUniform1i("uIsTwist", static_cast<int>(mIsStreakTwist));
    mStreakShader.setUniform1f("uScalex", mStreakScalex);
    mStreakShader.setUniform1f("uScaley", mStreakScaley);
    mStreakShader.setUniform1f("uSpeed", mStreakSpeed);
    mStreakShader.setUniform4f("uColor", mColor);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mStreakShader.end();
    mEffectFbo.end();

    return mEffectFbo;
}

void StreakPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void StreakPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}

void StreakPass::loadShaders()
{
    mStreakShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/streak/streak.frag");
}
