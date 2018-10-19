#pragma once

#include "ofFbo.h"

namespace ofxVFX
{
    class BaseRenderPass
    {
    public:
        BaseRenderPass()
        {};
        
        virtual ~BaseRenderPass() {};
        virtual void setup(const int width, const int height) = 0;
        virtual void update(const float time) = 0;
        virtual ofFbo& process(ofFbo& baseFbo) = 0;
        virtual void draw(float x, float y, float width, float height) = 0;
        
        // Common
        virtual inline void setColor(const ofFloatColor& color) {}
        
        // Bloom
        virtual inline void setBloomAttenuation(const float attenuation) {}
        virtual inline void setBloomBlurScale(const float blurScale) {}
        
        // Optical Flow
        virtual inline void setOpticalBlurScale(const float blurScale) {}
        virtual inline void setOpticalThresh(const float opticalThresh) {}
        virtual inline void setOpticalScale(const float opticalScale) {}
        virtual inline void setOpticalFade(const float opticalFade) {}
        virtual inline void setOpticalForce(const float opticalForce) {}
        virtual inline void setOpticalAmt(const float opticalAmt) {}
        
        // CRT
        virtual inline void setCRTDirection(const int CRTDirection) {}
        virtual inline void setCRTPixelSize(const float CRTPixelSize) {}
        
        // Symmetry
        virtual inline void setSymmetryDirection(const int symmetryDirection) {}
        
        // Streak
        virtual inline void setIsStreakAdd(const bool isStreakAdd) {}
        virtual inline void setIsStreakTwist(const bool isStreakTwist) {}
        virtual inline void setStreakScalex(const float streakScalex) {}
        virtual inline void setStreakScaley(const float streakScaley) {}
        virtual inline void setStreakSpeed(const float streakSpeed) {}
        
        // MNCA
        virtual inline void setMNCAColorMode(const int colorMode) {}
    };
}
