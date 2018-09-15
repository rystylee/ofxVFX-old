#pragma once

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "BaseRenderPass.hpp"

namespace ofxVFX
{
    class CRTPass : public BaseRenderPass
    {
    public:
        CRTPass();
        void setup(const int width, const int height) override;
        void update(const float time) override;
        ofFbo& process(ofFbo& base) override;
        void draw(const float x, const float y, const float width, const float height) override;
        
        inline void setCRTDirection(const int CRTDirection) override { mCRTDirection = CRTDirection; }
        inline void setCRTPixelSize(const float CRTPixelSize) override { mCRTPixelSize = CRTPixelSize; }
        
    private:
        void initBuffers();
        void loadShaders();
        
        int mWidth, mHeight;
        ofFbo mEffectFbo;
        
        ofShader mCRTShader;
        int mCRTDirection;
        float mCRTPixelSize;
    };
}
