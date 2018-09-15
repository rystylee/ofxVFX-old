#include "Scene.hpp"

// --------------------------------------------------------------
// Public
// --------------------------------------------------------------

Scene::Scene()
{}

void Scene::setup(const int width, const int height)
{
    mWidth = width, mHeight = height;
    
    ofSetBackgroundColor(0);
    ofEnableDepthTest();
    
    int R = 500;
    for(int i=0; i<100; i++)
    {
        mPositions.emplace_back(ofRandom(-R, R), ofRandom(-R, R), ofRandom(-R, R));
        mColors.emplace_back(ofRandom(1.0), ofRandom(1.0), ofRandom(1.0), 1.0);
    }
    
    initVFX();
    initGui();
}

void Scene::update(const float time)
{
    mTime = time;
    updateVFXParams();
}

void Scene::draw()
{
    mVFX.begin();
    {
        mCam.begin();
        ofPushMatrix();
        ofRotateYDeg(mTime * 20.0);
        {
            for(int i=0; i<mPositions.size(); i++)
            {
                ofPushStyle();
                ofSetColor(mColors[i]);
                ofDrawBox(mPositions[i], 50, 50, 50);
                ofPopStyle();
            }
        }
        ofPopMatrix();
        mCam.end();
    }
    mVFX.end();
    mVFX.process(mVFXMode);
    mVFX.draw(0, 0, mWidth, mHeight);
    
    if(mIsGui)
        drawGui();
}

void Scene::keyPressed(const int key)
{
    if(key == 'g')
        mIsGui = !mIsGui;
}

// --------------------------------------------------------------
// Private
// --------------------------------------------------------------

void Scene::initVFX()
{
    mVFX.addPass<ofxVFX::BloomPass>();
    mVFX.addPass<ofxVFX::OpticalFlowPass>();
    mVFX.addPass<ofxVFX::CRTPass>();
    mVFX.addPass<ofxVFX::EdgePass>();
    mVFX.addPass<ofxVFX::SymmetryPass>();
    mVFX.addPass<ofxVFX::StreakPass>();
    mVFX.addPass<ofxVFX::NoiseWarpPass>();
    mVFX.addPass<ofxVFX::CAPass>();
    mVFX.addPass<ofxVFX::InvertPass>();
    mVFX.addPass<ofxVFX::MNCAPass>();
    mVFX.addPass<ofxVFX::InkPass>();
    mVFX.addPass<ofxVFX::ZoomBlurPass>();
    
    mVFX.generate(mWidth, mHeight);
}

void Scene::initGui()
{
    mGui.setup();
    // Global
    mVFXGlobalGroup.setName("VFX Global Control");
    mVFXGlobalGroup.add(mVFXMode.set("VFX Mode", 0, 0, mVFX.getNumRenderPasses()- 1));
    mGui.add(mVFXGlobalGroup);
    // Blur
    mBlurGroup.setName("Blur Control");
    mBlurGroup.add(mBlurScale.set("Blur Scale", 1.0, 0.0, 5.0));
    mGui.add(mBlurGroup);
    // Bloom
    mBloomGroup.setName("Bloom Control");
    mBloomGroup.add(mBloomAttenuation.set("Bloom Attenuation", 1.0, 0.0, 10.0));
    mGui.add(mBloomGroup);
    // Optical Flow
    mOpticalGroup.setName("Optical Flow Control");
    mOpticalGroup.add(mOpticalThresh.set("Optical Thresh", 0.5, 0.0, 1.0));
    mOpticalGroup.add(mOpticalScale.set("Optical Scale", 5.0, 0.0, 10.0));
    mOpticalGroup.add(mOpticalFade.set("Optical Fade", 0.99, 0.0, 1.5));
    mOpticalGroup.add(mOpticalForce.set("Optical Force", 0.6, 0.0, 1.5));
    mOpticalGroup.add(mOpticalAmt.set("Optical Amt", 1.0, 0.0, 1.5));
    mGui.add(mOpticalGroup);
    // CRT
    mCRTGroup.setName("CRT Control");
    mCRTGroup.add(mCRTDirection.set("CRT Direction", 0, 0, 1));
    mCRTGroup.add(mCRTPixelSize.set("CRT PixelSize", 4.0, 1.0, 20.0));
    mGui.add(mCRTGroup);
    // Symmetry
    mSymmetryGroup.setName("Symmetry Control");
    mSymmetryGroup.add(mSymmetryDirection.set("Symmetry Direction", 0, 0, 1));
    mGui.add(mSymmetryGroup);
    // Streak
    mStreakGroup.setName("Streak Control");
    mStreakGroup.add(mIsStreakAdd.set("Is StreakAdd", true));
    mStreakGroup.add(mIsStreakTwist.set("Is StreakTwist", false));
    mStreakGroup.add(mStreakScalex.set("Streak ScaleX", 0.01, 0.0, 1.0));
    mStreakGroup.add(mStreakScaley.set("Streak ScaleY", 0.0, 0.0, 1.0));
    mStreakGroup.add(mStreakSpeed.set("Streak Speed", 1.0, 0.0, 10.0));
    mGui.add(mStreakGroup);
    // MNCA
    mMNCAGroup.setName("MNCA Control");
    mMNCAGroup.add(mMNCAColorMode.set("MNCA ColorMode", 0, 0, 3));
    mGui.add(mMNCAGroup);
}

void Scene::updateVFXParams()
{
    mVFX.update(mTime);
    // Blur
    mVFX.getRenderPassPtr(0)->setBloomBlurScale(mBlurScale);
    mVFX.getRenderPassPtr(1)->setBloomBlurScale(mBlurScale);
    // Bloom
    mVFX.getRenderPassPtr(0)->setBloomAttenuation(mBloomAttenuation);
    // Optical Flow
    mVFX.getRenderPassPtr(1)->setOpticalThresh(mOpticalThresh);
    mVFX.getRenderPassPtr(1)->setOpticalScale(mOpticalScale);
    mVFX.getRenderPassPtr(1)->setOpticalFade(mOpticalFade);
    mVFX.getRenderPassPtr(1)->setOpticalForce(mOpticalForce);
    mVFX.getRenderPassPtr(1)->setOpticalAmt(mOpticalAmt);
    // CRT
    mVFX.getRenderPassPtr(2)->setCRTDirection(mCRTDirection);
    mVFX.getRenderPassPtr(2)->setCRTPixelSize(mCRTPixelSize);
    // Symmetry
    mVFX.getRenderPassPtr(4)->setSymmetryDirection(mSymmetryDirection);
    // Streak
    mVFX.getRenderPassPtr(5)->setIsStreakAdd(mIsStreakAdd);
    mVFX.getRenderPassPtr(5)->setIsStreakTwist(mIsStreakTwist);
    mVFX.getRenderPassPtr(5)->setStreakScalex(mStreakScalex);
    mVFX.getRenderPassPtr(5)->setStreakScaley(mStreakScaley);
    mVFX.getRenderPassPtr(5)->setStreakSpeed(mStreakSpeed);
    // MNCA
    mVFX.getRenderPassPtr(9)->setMNCAColorMode(mMNCAColorMode);
}

void Scene::drawGui()
{
    ofDisableDepthTest();
    mGui.draw();
    ofEnableDepthTest();
}
