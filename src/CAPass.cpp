#include "CAPass.hpp"

using namespace ofxVFX;

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

CAPass::CAPass()
{
    // ------------------------------------------------------
    // reference : https://www.shadertoy.com/view/4dsGDn
    // ------------------------------------------------------
    std::string frag = GLSL410(
                               precision mediump float;
                               
                               in vec4 vPosition;
                               
                               out vec4 fragColor;
                               
                               uniform sampler2DRect uBase;
                               uniform float uTime;
                               uniform float uRandVal;
                               
                               float hash(in float n)
                               {
                                   return fract(sin(n) * 43758.5453123);
                               }
                               
                               void main()
                               {
                                   vec2 uv = vPosition.xy;
                                   
                                   float t = pow((((1.0 + sin(uTime * 10.0) * 0.5)
                                                   * 0.8 + sin(uTime * cos(uv.y) * 41415.92653) * 0.0125)
                                                  * 1.5 + sin(uTime * 7.0) * 0.5), 5.0);
                                   
                                   if(uRandVal < 0.5)
                                       t = t;
                                   else
                                       t = -t;
                                   
                                   vec4 c1 = texture(uBase, uv + vec2(t * 0.2, 0.0));
                                   vec4 c2 = texture(uBase, uv + vec2(t * 0.5, 0.0));
                                   vec4 c3 = texture(uBase, uv + vec2(t * 0.9, 0.0));
                                   
                                   float noise = hash((hash(uv.x) + uv.t) * uTime) * 0.055;
                                   
                                   fragColor = vec4(vec3(c3.r, c2.g, c1.b) + noise, 1.0);
                               }
                               );
    
    mCAShader.setupShaderFromSource(GL_VERTEX_SHADER, mPassVert);
    mCAShader.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
    mCAShader.bindDefaults();
    mCAShader.linkProgram();
}

void CAPass::setup(const int width, const int height)
{
    mWidth = width; mHeight = height;
    initBuffers();
}

void CAPass::update(const float time)
{
    mTime = time;
}

ofFbo& CAPass::process(ofFbo& baseFbo)
{
    mEffectFbo.begin();
    ofClear(0);
    mCAShader.begin();
    mCAShader.setUniformTexture("uBase", baseFbo.getTexture(), 0);
    mCAShader.setUniform1f("uTime", mTime);
    mCAShader.setUniform1f("uRandVal", ofRandom(1.0));
    ofDrawRectangle(0, 0, mWidth, mHeight);
    mCAShader.end();
    mEffectFbo.end();
    
    return mEffectFbo;
}

void CAPass::draw(const float x, const float y, const float width, const float height)
{
    mEffectFbo.draw(x, y, width, height);
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void CAPass::initBuffers()
{
    mEffectFbo.allocate(mWidth, mHeight);
}
