#pragma once

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "ofMath.h"
#include "BaseRenderPass.hpp"

#include <array>

namespace ofxVFX
{
    class CAPass : public BaseRenderPass
    {
    public:
        CAPass();
        void setup(const int width, const int height) override;
        void update(const float time) override;
        ofFbo& process(ofFbo& base) override;
        void draw(const float x, const float y, const float width, const float height) override;
        
    private:
        void initBuffers();

        int mWidth, mHeight;
        ofFbo mEffectFbo;
        
        float mTime;
        
        ofShader mCAShader;
    };
}
