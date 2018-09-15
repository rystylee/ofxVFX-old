#pragma once

#include "ofGraphics.h"
#include "ofFbo.h"
#include "ofShader.h"
#include "BaseRenderPass.hpp"

namespace ofxVFX
{
    class StreakPass : public BaseRenderPass
    {
    public:
        StreakPass();
        void setup(const int width, const int height) override;
        void update(const float time) override;
        ofFbo& process(ofFbo& base) override;
        void draw(const float x, const float y, const float width, const float height) override;
        
        inline void setColor(const ofFloatColor& color) override { mColor = color; }
        inline void setIsStreakAdd(const bool isStreakAdd) override { mIsStreakAdd = isStreakAdd; }
        inline void setIsStreakTwist(const bool isStreakTwist) override { mIsStreakTwist = isStreakTwist; }
        inline void setStreakScalex(const float streakScalex) override { mStreakScalex = streakScalex; }
        inline void setStreakScaley(const float streakScaley) override { mStreakScaley = streakScaley; }
        inline void setStreakSpeed(const float streakSpeed) override { mStreakSpeed = streakSpeed; }
        
    private:
        void initBuffers();
        void loadShaders();
        
        int mWidth, mHeight;
        ofFbo mEffectFbo;
        
        float mTime;
        
        ofShader mStreakShader;
        ofFloatColor mColor;
        bool mIsStreakAdd;
        bool mIsStreakTwist;
        float mStreakScalex;
        float mStreakScaley;
        float mStreakSpeed;
    };
}
