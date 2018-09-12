#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    mScene.setup(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    mScene.update(ofGetElapsedTimef());
}

//--------------------------------------------------------------
void ofApp::draw()
{
    mScene.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    mScene.keyPressed(key);
}

//--------------------------------------------------------------
