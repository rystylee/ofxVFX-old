#include "InkPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

InkPass::InkPass()
{}

void InkPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void InkPass::update(const float time)
{
    mTime = time;
}

ofFbo& InkPass::process(ofFbo& baseFbo)
{
    mInkPingPong.dst->begin();
    ofClear(0);
    mInkShader.begin();
    mInkShader.setUniformTexture("uPrevBuffer", mInkPingPong.src->getTexture(), 0);
    mInkShader.setUniformTexture("uBase", baseFbo.getTexture(), 1);
    mInkShader.setUniform1f("uTime", mTime);
    mInkShader.setUniform2f("uResolution", mWidth, mHeight);
    mInkShader.setUniform1i("uCount", ofGetFrameNum());
    mInkPingPong.src->draw(0, 0, mWidth, mHeight);
    mInkShader.end();
    mInkPingPong.dst->end();
    mInkPingPong.swap();
    
    mEffectFbo.begin();
    ofClear(0);
    mInkRenderShader.begin();
    mInkRenderShader.setUniformTexture("uResult", mInkPingPong.dst->getTexture(), 0);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mInkRenderShader.end();
    mEffectFbo.end();

    return mEffectFbo;
}

void InkPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void InkPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
    
    auto inkColor = std::make_unique<float[]>(mWidth * mHeight * 4);
    for(int x=0; x<mWidth; x++)
    {
        for(int y=0; y<mHeight; y++)
        {
            const int i = x * mHeight + y;
            inkColor[i * 4 + 0] = 0.0;
            inkColor[i * 4 + 1] = 0.0;
            inkColor[i * 4 + 2] = 0.0;
            inkColor[i * 4 + 3] = 0.0;
        }
    }
    mInkPingPong.allocate(mWidth, mHeight, GL_RGBA);
    mInkPingPong.src->getTexture().loadData(inkColor.get(), mWidth, mHeight, GL_RGBA);
    mInkPingPong.dst->getTexture().loadData(inkColor.get(), mWidth, mHeight, GL_RGBA);
}

void InkPass::loadShaders()
{
    mInkShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/ink/ink.frag");
    mInkRenderShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/ink/render.frag");
}
