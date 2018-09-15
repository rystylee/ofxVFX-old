#pragma once

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "BaseRenderPass.hpp"
#include "ofEvents.h"
#include "Common.hpp"

#include <memory>

namespace ofxVFX
{
    class InkPass : public BaseRenderPass
    {
    public:
        InkPass();
        void setup(const int width, const int height) override;
        void update(const float time) override;
        ofFbo& process(ofFbo& base) override;
        void draw(const float x, const float y, const float width, const float height) override;
        
    private:
        void initBuffers();
        void loadShaders();
        
        int mWidth, mHeight;
        ofFbo mEffectFbo;
        
        float mTime;
        
        PingPongBuffer mInkPingPong;
        ofShader mInkShader;
        ofShader mInkRenderShader;
    };
}
