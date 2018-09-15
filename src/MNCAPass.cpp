#include "MNCAPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

MNCAPass::MNCAPass()
: mMNCAColorMode(0)
{
    mNoiseTex.load("textures/ofxVFX/mnca/noise.png");
}

void MNCAPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void MNCAPass::update(const float time)
{
    mTime = time;
}

ofFbo& MNCAPass::process(ofFbo& baseFbo)
{
    if(ofRandom(1.0) < 0.2)
    {
        mMNCAPingPong.dst->begin();
        ofClear(0);
        ofPushStyle();
        ofSetColor(255);
        baseFbo.draw(0, 0, mWidth, mHeight);
        mMNCAPingPong.dst->end();
        mMNCAPingPong.swap();
        ofPopStyle();
    }
    
    mMNCAPingPong.dst->begin();
    ofClear(0);
    mMNCA0Shader.begin();
    mMNCA0Shader.setUniformTexture("uPrevBuffer", mMNCAPingPong.src->getTexture(), 0);
    mMNCA0Shader.setUniformTexture("uNoiseTex", mNoiseTex.getTexture(), 1);
    mMNCA0Shader.setUniform1i("uIsReset", mIsMNCAReset);
    mMNCAPingPong.src->draw(0, 0, mWidth, mHeight);
    mMNCA0Shader.end();
    mMNCAPingPong.dst->end();
    mMNCAPingPong.swap();
    
    mEffectFbo.begin();
    ofClear(0);
    mMNCARenderShader.begin();
    mMNCARenderShader.setUniformTexture("uSimuTex", mMNCAPingPong.dst->getTexture(), 0);
    mMNCARenderShader.setUniformTexture("uBase", baseFbo.getTexture(), 1);
    mMNCARenderShader.setUniform2f("uResolution", mWidth, mHeight);
    mMNCARenderShader.setUniform1i("uColorMode", mMNCAColorMode);
    mMNCARenderShader.setUniform1f("uTime", mTime);
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mMNCARenderShader.end();
    mEffectFbo.end();
    
    mIsMNCAReset++;

    return mEffectFbo;
}

void MNCAPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void MNCAPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
    
    auto mncaColor = std::make_unique<float[]>(mWidth * mHeight * 3);
    for(int x=0; x<mWidth; x++)
    {
        for(int y=0; y<mHeight; y++)
        {
            const int i = x * mHeight + y;
            mncaColor[i * 3 + 0] = ofRandom(1.0);
            mncaColor[i * 3 + 1] = ofRandom(1.0);
            mncaColor[i * 3 + 2] = ofRandom(1.0);
        }
    }
    mMNCAPingPong.allocate(mWidth, mHeight, GL_RGB32F);
    mMNCAPingPong.src->getTexture().loadData(mncaColor.get(), mWidth, mHeight, GL_RGB32F);
    mMNCAPingPong.dst->getTexture().loadData(mncaColor.get(), mWidth, mHeight, GL_RGB32F);
}

void MNCAPass::loadShaders()
{
    mMNCA0Shader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/MNCA/mnca0.frag");
    mMNCARenderShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/MNCA/render.frag");
}
