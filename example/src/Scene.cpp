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
    
    int R = 800;
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
                ofDrawSphere(mPositions[i], 30);
                ofPopStyle();
            }
        }
        ofPopMatrix();
        mCam.end();
    }
    mVFX.end();
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
    mVFX.addPass<ofxVFX::CAPass>();
    mVFX.addPass<ofxVFX::DistortionPass>();
    mVFX.addPass<ofxVFX::EdgePass>();
    mVFX.addPass<ofxVFX::StreakPass>();
    mVFX.addPass<ofxVFX::ZoomBlurPass>();
    mVFX.addPass<ofxVFX::MNCAPass>();
    mVFX.addPass<ofxVFX::InvertPass>();

    mVFX.generate(mWidth, mHeight);
    
    mVFX.setRenderPassIndex(0);
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
    // Streak
    mStreakGroup.setName("Streak Control");
    mStreakGroup.add(mStreakColor.set("Streak Color", ofFloatColor(1.0), ofFloatColor(0.0), ofFloatColor(1.0)));
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
    mVFX.setRenderPassIndex(mVFXMode);
    mVFX.update(mTime);

    // Blur
    mVFX.getRenderPassPtr(0)->setBloomBlurScale(mBlurScale);
    mVFX.getRenderPassPtr(1)->setBloomBlurScale(mBlurScale);
    // Bloom
    mVFX.getRenderPassPtr(0)->setBloomAttenuation(mBloomAttenuation);
    // Streak
    mVFX.getRenderPassPtr(4)->setColor(mStreakColor);
    mVFX.getRenderPassPtr(4)->setIsStreakAdd(mIsStreakAdd);
    mVFX.getRenderPassPtr(4)->setIsStreakTwist(mIsStreakTwist);
    mVFX.getRenderPassPtr(4)->setStreakScalex(mStreakScalex);
    mVFX.getRenderPassPtr(4)->setStreakScaley(mStreakScaley);
    mVFX.getRenderPassPtr(4)->setStreakSpeed(mStreakSpeed);
    // MNCA
    mVFX.getRenderPassPtr(6)->setMNCAColorMode(mMNCAColorMode);
}

void Scene::drawGui()
{
    ofDisableDepthTest();
    mGui.draw();
    ofEnableDepthTest();
}
