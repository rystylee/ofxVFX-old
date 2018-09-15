#pragma once

#include <vector>
#include <memory>

#include "ofGraphics.h"
#include "ofFbo.h"

#include "BaseRenderPass.hpp"

namespace ofxVFX
{
    class VFX
    {
    public:
        VFX();
        void generate(const int width, const int height);
        void update(const float time);
        void begin();
        void end();
        void process(const int passIndex);
        void draw(const float x, const float y, const float width, const float height);
        
        template<class T>
        void addPass()
        {
            mRenderPasses.emplace_back(std::make_unique<T>());
        }
        
        BaseRenderPass* getRenderPassPtr(const int renderPassIndex)
        {
            BaseRenderPass* renderPassPtr = mRenderPasses.at(renderPassIndex).get();
            return renderPassPtr;
        }
      
        int getNumRenderPasses() { return mRenderPasses.size(); }

    private:
        ofFbo mPingFbo, mPongFbo;
        std::vector<std::unique_ptr<BaseRenderPass>> mRenderPasses;
    };
}
