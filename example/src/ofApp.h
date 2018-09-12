#pragma once

#include "ofMain.h"
#include "Scene.hpp"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    Scene mScene;
};
