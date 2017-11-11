#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "switches.h"

class ofApp : public ofBaseApp {
	int yOffsetIncr(const int& d = 20);
	void colorFaces();

	int vidWidth;
	int vidHeight;
	int nChannels;
	ofVideoPlayer player;
	ofImage imgOrig, imgResized;
	float resizeRate = 1.f; // resized image is passed to face finder, expecting faster operation

	bool frameByframe;

	unsigned char hue;
	int yOffset;

	ofxCvHaarFinder finder;

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

};

