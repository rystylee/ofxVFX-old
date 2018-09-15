#pragma once

#include "ofFbo.h"

namespace ofxVFX
{
    class PingPongBuffer
    {
    public:
        void allocate(const int width, const int height, const int internalformat = GL_RGBA, const int numColorBuffers = 1)
        {
            // Allocate
            ofFbo::Settings s;
            s.width  = width;
            s.height = height;
            s.numColorbuffers = numColorBuffers;
            s.useDepth = false;
            s.internalformat = internalformat;    // Gotta store the data as floats, they won't be clamped to 0..1
            s.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
            s.wrapModeVertical = GL_CLAMP_TO_EDGE;
            s.minFilter = GL_NEAREST; // No interpolation, that would mess up data reads later!
            s.maxFilter = GL_NEAREST;
            for(int i=0; i<2; i++)
            {
                FBOs[i].allocate(s);
            }
            
            // Clean
            clear();
        }
        
        void swap()
        {
            std::swap(src, dst);
        }
        
        void clear()
        {
            for(int i=0; i<2; i++)
            {
                FBOs[i].begin();
                ofClear(0, 255);
                FBOs[i].end();
            }
        }
        
        ofFbo& operator[](int n) { return FBOs[n]; }
        ofFbo* src = &FBOs[0];       // Source       ->  Ping
        ofFbo* dst = &FBOs[1];       // Destination  ->  Pong
        
    private:
        ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s
    };
}
