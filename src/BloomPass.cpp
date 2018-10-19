#include "BloomPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

BloomPass::BloomPass()
: mAttenuation(1.0), mBlurScale(1.0)
{}

void BloomPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
    loadShaders();
}

void BloomPass::update(const float time)
{}

ofFbo& BloomPass::process(ofFbo& baseFbo)
{
    // Brightness Threshold Pass
    mBrightnessThreshFbo.begin();
    ofClear(0, 255);
    mBrightnessThreshShader.begin();
    mBrightnessThreshShader.setUniform2f("uResolution", mWidth, mHeight);
    mBrightnessThreshShader.setUniform1f("uBrightnessThresh", 0.3);
    baseFbo.draw(0, 0, mWidth, mHeight);
    mBrightnessThreshShader.end();
    mBrightnessThreshFbo.end();
    
    // Vertical Blur Pass
    mBlurFbo[0].begin();
    ofClear(0, 255);
    mBlurShader.begin();
    mBlurShader.setUniform2f("uResolution", mWidth, mHeight);
    mBlurShader.setUniform1i("uDirection", 0);
    mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
    mBlurShader.setUniform1f("uAttenuation", mAttenuation);
    mBrightnessThreshFbo.draw(0, 0, mWidth, mHeight);
    mBlurShader.end();
    mBlurFbo[0].end();
    
    // Horizontal Blur Pass
    mBlurFbo[1].begin();
    ofClear(0, 255);
    mBlurShader.begin();
    mBlurShader.setUniform2f("uResolution", mWidth, mHeight);
    mBlurShader.setUniform1i("uDirection", 1);
    mBlurShader.setUniform1f("uOffsetScale", mBlurScale);
    mBlurShader.setUniform1f("uAttenuation", mAttenuation);
    mBlurFbo[0].draw(0, 0, mWidth, mHeight);
    mBlurShader.end();
    mBlurFbo[1].end();
    
    // Composite Pass
    mCompositeFbo.begin();
    ofClear(0, 255);
    mBloomCompositeShader.begin();
    mBloomCompositeShader.setUniformTexture("uBlur", mBlurFbo[1].getTexture(), 1);
    mBloomCompositeShader.setUniform2f("uResolution", mWidth, mHeight);
    mBloomCompositeShader.setUniform1f("uGamma", 2.2);
    baseFbo.draw(0, 0, mWidth, mHeight);
    mBloomCompositeShader.end();
    mCompositeFbo.end();
    
    // Final
    mEffectFbo.begin();
    ofClear(0, 255);
    mCompositeFbo.draw(0, 0, mWidth, mHeight);
    mEffectFbo.end();
    
    return mEffectFbo;
}

void BloomPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void BloomPass::initBuffers()
{
    ofFboSettings s;
    s.width = mWidth;
    s.height = mHeight;
    s.numColorbuffers = 1;
    s.useDepth = true;
    s.useStencil = true;
    s.textureTarget = GL_TEXTURE_2D;
    s.internalformat = GL_RGBA32F;
    s.depthStencilAsTexture = true;
    s.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    s.wrapModeVertical = GL_CLAMP_TO_EDGE;
    s.minFilter = GL_NEAREST;
    s.maxFilter = GL_NEAREST;
    s.numSamples = 0;
    
    mEffectFbo.allocate(s);
    
    s.internalformat = GL_RGBA;
    mBrightnessThreshFbo.allocate(s);
    mBlurFbo[0].allocate(s);
    mBlurFbo[1].allocate(s);
    mCompositeFbo.allocate(s);
}

void BloomPass::loadShaders()
{
    mBrightnessThreshShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/brightnessThresh.frag");
    mBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/blur.frag");
    mBloomCompositeShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/bloom/composite.frag");
}
