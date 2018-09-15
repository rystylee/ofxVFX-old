#pragma once

#include "ofFbo.h"

#define GLSL410(shader) "#version 410 \n" #shader

namespace ofxVFX
{
    class BaseRenderPass
    {
    public:
        BaseRenderPass()
        {
            mPassVert = GLSL410(
                                // -----------------------------
                                // oF default
                                // -----------------------------
                                in vec4 color;
                                in vec4 position;
                                in vec4 normal;
                                in vec2 texcoord;
                                
                                out vec4 vColor;
                                out vec4 vPosition;
                                out vec4 vNormal;
                                out vec2 vTexCoord;
                                
                                // -----------------------------
                                // oF default
                                // -----------------------------
                                uniform mat4 modelViewProjectionMatrix;
                                uniform mat4 modelViewMatrix;
                                
                                void main()
                                {
                                    vColor = color;
                                    vPosition = position;
                                    vNormal = normal;
                                    vTexCoord = texcoord;
                                    gl_Position = modelViewProjectionMatrix * position;
                                }
                                );
        };
        
        virtual ~BaseRenderPass() {};
        virtual void setup(const int width, const int height) = 0;
        virtual void update(const float time) = 0;
        virtual ofFbo& process(ofFbo& baseFbo) = 0;
        virtual void draw(float x, float y, float width, float height) = 0;
        
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
        virtual inline void setColor(const ofFloatColor& color) {}
        virtual inline void setIsStreakAdd(const bool isStreakAdd) {}
        virtual inline void setIsStreakTwist(const bool isStreakTwist) {}
        virtual inline void setStreakScalex(const float streakScalex) {}
        virtual inline void setStreakScaley(const float streakScaley) {}
        virtual inline void setStreakSpeed(const float streakSpeed) {}
        
        // MNCA
        virtual inline void setMNCAColorMode(const int colorMode) {}
            
        
    protected:
        std::string mPassVert;
    };
}
