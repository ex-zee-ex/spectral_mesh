/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOMXVideoGrabber.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void midibiz();
    
    void omx_settings();
	
	void omx_updates();
    
    void gridmesh(int gridsize);
    void trianglemesh(int gridsize);
    void horizontal_linemesh(int gridsize);
    void vertical_linemesh(int gridsize);
    
    ofImage perlin_noise(float theta, float resolution, ofImage noise_image);
    
    //----------------------------midibiz
    
	void newMidiMessage(ofxMidiMessage& eventArgs);
	
	ofxMidiIn midiIn;
	std::vector<ofxMidiMessage> midiMessages;
	std::size_t maxMessages = 10; //< max number of messages to keep track of
    
    
    
    ofVideoGrabber cam1;
    
    ofMesh mesh1;
    
    ofVboMesh vbo_mesh1;
    
    //ofVboMesh vbo_mesh2;
   
    ofShader shader_displace;
    
    ofFbo fbo0;
    
    ofxOMXCameraSettings settings;
    ofxOMXVideoGrabber videoGrabber;
    
    ofImage x_noise_image;
    ofImage y_noise_image;
    ofImage z_noise_image;
   
    
};
