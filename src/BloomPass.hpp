#pragma once

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "BaseRenderPass.hpp"

#include <array>

namespace ofxVFX
{
    class BloomPass : public BaseRenderPass
    {
    public:
        BloomPass();
        void setup(const int width, const int height) override;
        void update(const float time) override;
        ofFbo& process(ofFbo& baseFbo) override;
        void draw(const float x, const float y, const float width, const float height) override;
        
        inline void setBloomAttenuation(const float attenuation) override { mAttenuation = attenuation; }
        inline void setBloomBlurScale(const float blurScale) override { mBlurScale = blurScale; }

    private:
        void initBuffers();
        void loadShaders();
        
        int mWidth, mHeight;
        ofFbo mEffectFbo;
        
        ofFbo mBrightnessThreshFbo, mCompositeFbo;
        std::array<ofFbo, 2> mBlurFbo;
        ofShader mBrightnessThreshShader, mBlurShader, mBloomCompositeShader; // share with Optical flow
        float mAttenuation, mBlurScale;
    };
}
