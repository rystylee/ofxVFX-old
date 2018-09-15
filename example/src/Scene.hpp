#pragma once

#include "ofMain.h"
#include "ofxVFX.h"
#include "ofxGui.h"

class Scene
{
public:
    Scene();
    void setup(const int width, const int height);
    void update(const float time);
    void draw();
    void keyPressed(const int key);
    
private:
    void initVFX();
    void initGui();
    void updateVFXParams();
    void drawGui();
    
    int mWidth, mHeight;
    float mTime;
    
    ofEasyCam mCam;
    vector<glm::vec3> mPositions;
    vector<ofFloatColor> mColors;
    
    ofxVFX::VFX mVFX;
    vector<string> mVFXRenderPassNames;
   
    bool mIsGui = true;
    
    ofxPanel mGui;
    // Global
    ofParameterGroup mVFXGlobalGroup;
    ofParameter<int> mVFXMode;
    // Blur
    ofParameterGroup mBlurGroup;
    ofParameter<float> mBlurScale;
    // Bloom
    ofParameterGroup mBloomGroup;
    ofParameter<float> mBloomAttenuation;
    // Optical Flow
    ofParameterGroup mOpticalGroup;
    ofParameter<float> mOpticalThresh;
    ofParameter<float> mOpticalScale;
    ofParameter<float> mOpticalFade;
    ofParameter<float> mOpticalForce;
    ofParameter<float> mOpticalAmt;
    // CRT
    ofParameterGroup mCRTGroup;
    ofParameter<int> mCRTDirection;
    ofParameter<float> mCRTPixelSize;
    // Symmetry
    ofParameterGroup mSymmetryGroup;
    ofParameter<int> mSymmetryDirection;
    // Streak
    ofParameterGroup mStreakGroup;
    ofParameter<bool> mIsStreakAdd;
    ofParameter<bool> mIsStreakTwist;
    ofParameter<float> mStreakScalex;
    ofParameter<float> mStreakScaley;
    ofParameter<float> mStreakSpeed;
    // MNCA
    ofParameterGroup mMNCAGroup;
    ofParameter<int> mMNCAColorMode;
};
