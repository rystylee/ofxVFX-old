#include "VFX.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

VFX::VFX()
{}

void VFX::generate(const int width, const int height)
{
    mPingFbo.allocate(width, height);
    mPongFbo.allocate(width, height);
    
    for(auto& e : mRenderPasses)
    {
        e.get()->setup(width, height);
    }
}

void VFX::update(const float time)
{
    for(auto& e : mRenderPasses)
    {
        e.get()->update(time);
    }
}

void VFX::begin()
{
    mPingFbo.begin();
    ofClear(0);
}

void VFX::end()
{
    mPingFbo.end();
}

void VFX::process(const int passIndex)
{
    mPongFbo = mRenderPasses.at(passIndex).get()->process(mPingFbo);
}

void VFX::draw(const float x, const float y, const float width, const float height)
{
    mPongFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------
