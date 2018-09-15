#pragma once

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "BaseRenderPass.hpp"
#include "ofMath.h"
#include "ofImage.h"
#include "Common.hpp"

#include <memory>

namespace ofxVFX
{
    class MNCAPass : public BaseRenderPass
    {
    public:
        MNCAPass();
        void setup(const int width, const int height) override;
        void update(const float time) override;
        ofFbo& process(ofFbo& base) override;
        void draw(const float x, const float y, const float width, const float height) override;
        
        inline void setMNCAColorMode(const int colorMode) override { mMNCAColorMode = colorMode;  }
        
    private:
        void initBuffers();
        void loadShaders();
        
        int mWidth, mHeight;
        ofFbo mEffectFbo;
        
        float mTime;
        
        ofShader mMNCA0Shader;
        ofShader mMNCARenderShader;
        ofShader mMNCACompositeShader;
        PingPongBuffer mMNCAPingPong;
        int mIsMNCAReset;
        int mMNCAColorMode;
        ofImage mNoiseTex;
    };
}
