#include "VFX.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

VFX::VFX()
: mRenderPassIndex(0)
{}

void VFX::generate(const int width, const int height)
{
    ofFboSettings s;
    s.width = width;
    s.height = height;
    s.numColorbuffers = 1;
    s.useDepth = true;
    s.useStencil = true;
    s.textureTarget = GL_TEXTURE_2D;
    s.internalformat = GL_RGBA;
    s.depthStencilAsTexture = true;
    s.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    s.wrapModeVertical = GL_CLAMP_TO_EDGE;
    s.minFilter = GL_NEAREST;
    s.maxFilter = GL_NEAREST;
    s.numSamples = 0;
    
    mBaseFbo.allocate(s);
    mResultFbo.allocate(s);
    
    for(auto& e : mRenderPasses)
    {
        e.get()->setup(width, height);
    }
}

void VFX::update(const float time)
{
    mRenderPasses[mRenderPassIndex].get()->update(time);
}

void VFX::begin()
{
    mBaseFbo.begin();
    ofClear(0, 0);
}

void VFX::end()
{
    mBaseFbo.end();
    mResultFbo = mRenderPasses[mRenderPassIndex].get()->process(mBaseFbo);
}

void VFX::draw(const float x, const float y, const float width, const float height)
{
    mResultFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------
