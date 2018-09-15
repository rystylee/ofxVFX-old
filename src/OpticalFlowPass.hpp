#pragma once

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "BaseRenderPass.hpp"
#include "Common.hpp"

#include <array>
#include <memory>

namespace ofxVFX
{
    class OpticalFlowPass : public BaseRenderPass
    {
    public:
        OpticalFlowPass();
        void setup(const int width, const int height) override;
        void update(const float time) override;
        ofFbo& process(ofFbo& base) override;
        void draw(const float x, const float y, const float width, const float height) override;
        
        inline void setOpticalBlurScale(const float blurScale) override { mBlurScale = blurScale; }
        inline void setOpticalThresh(const float opticalThresh) override { mOpticalThresh = opticalThresh; }
        inline void setOpticalScale(const float opticalScale) override { mOpticalScale = opticalScale; }
        inline void setOpticalFade(const float opticalFade) override { mOpticalFade = opticalFade; }
        inline void setOpticalForce(const float opticalForce) override { mOpticalForce = opticalForce; }
        inline void setOpticalAmt(const float opticalAmt) override { mOpticalAmt = opticalAmt; }
        
    private:
        void initBuffers();
        void loadShaders();
        
        int mWidth, mHeight;
        ofFbo mEffectFbo;
        
        float mTime;
        
        ofFbo mBackBuffer;
        std::array<ofFbo, 2> mBlurFbo;
        ofFbo mCompositeFbo;
        PingPongBuffer mFlowPingPong;
        ofShader mFlowShader, mBlurShader, mFlowRenderShader; // and used the mBlurShader
        float mBlurScale, mOpticalThresh, mOpticalScale, mOpticalFade, mOpticalForce, mOpticalAmt;
    };
}
