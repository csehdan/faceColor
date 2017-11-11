#include "ofApp.h"
#include "pixelcolor.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	frameByframe = false;

	resizeRate = 2;
	if (!player.load("movies/scene.mp4")) {
		std::cout << "Couldn't load video file. Exiting." << std::endl;
		std::exit(0);
	}

	vidWidth = player.getWidth();
	vidHeight = player.getHeight();
	nChannels = player.getPixels().getNumChannels();

	imgOrig.allocate(vidWidth, vidHeight, OF_IMAGE_COLOR);

	finder.setup("haarcascade_frontalface_default.xml");
	player.setLoopState(OF_LOOP_NORMAL);
	player.play();
}

//--------------------------------------------------------------
void ofApp::update() {
	player.update();
	imgOrig.setFromPixels(player.getPixels());
	colorFaces();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0xFFFFFF);

	int xOffset = vidWidth + vidWidth / 5;
	yOffset = vidHeight + vidHeight / 5;


	imgOrig.draw(20, 20);

#if DEBUG
	ofPushStyle();
	ofSetHexColor(0xff0000);
	ofNoFill();
	for (int i = 0; i < finder.blobs.size(); i++) {
		ofRectangle rct = finder.blobs[i].boundingRect;
		rct.scale(resizeRate);
		rct.x *= resizeRate;
		rct.y *= resizeRate;
		ofDrawRectangle(rct + ofPoint(20, 20));
	}
	ofPopStyle();
#endif // DEBUG

	ofSetHexColor(0x000000);
	ofDrawBitmapString("color hue: " + ofToString((int)hue, 2), 20, yOffsetIncr());
	ofDrawBitmapString("press f to change", 20, yOffsetIncr());
	if (frameByframe) ofSetHexColor(0xCCCCCC);
	ofDrawBitmapString("mouse position sets color of faces", 20, yOffsetIncr());

	if (!frameByframe) ofSetHexColor(0xCCCCCC); else ofSetHexColor(0x000000);
	ofDrawBitmapString("keys <- -> frame by frame ", 300, yOffsetIncr(0));
	ofSetHexColor(0x000000);

	ofDrawBitmapString("frame: " + ofToString(player.getCurrentFrame()) + "/" + ofToString(player.getTotalNumFrames()), 20, yOffsetIncr(40));
	ofDrawBitmapString("duration: " + ofToString(player.getPosition()*player.getDuration(), 2) + "/" + ofToString(player.getDuration(), 2), 20, yOffsetIncr());
	float fps = ofGetFrameRate();
	ofDrawBitmapString("Current fps: " + ofToString(fps, 2), 20, yOffsetIncr());

	if (player.getIsMovieDone()) {
		ofSetHexColor(0xFF0000);
		ofDrawBitmapString("end of movie", 20, yOffsetIncr());
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		frameByframe = !frameByframe;
		player.setPaused(frameByframe);
		break;
	case OF_KEY_LEFT:
		player.previousFrame();
		break;
	case OF_KEY_RIGHT:
		player.nextFrame();
		break;
	case '0':
		player.firstFrame();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	int width = ofGetWidth();
	float pct = (float)x / (float)width;
	hue = pct * 255;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (!frameByframe) {
		int width = ofGetWidth();
		float pct = (float)x / (float)width;
		player.setPosition(pct);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (!frameByframe) {
		player.setPaused(true);
	}
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (!frameByframe) {
		player.setPaused(false);
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

int ofApp::yOffsetIncr(const int& d) {
	yOffset += d;
	return yOffset;
}

void ofApp::colorFaces() {
	if (resizeRate <= 0) {
		std::cout << "resize rate is <=0, cannot do that..." << std::endl;
		return;
	}
	imgResized = imgOrig;
	imgResized.resize(vidWidth / resizeRate, vidHeight / resizeRate);
	finder.findHaarObjects(imgResized);
	ofPixels &pixels = player.getPixelsRef();
	ofPixels &origRef = imgOrig.getPixelsRef();
	for (auto blob : finder.blobs) {
		ofRectangle rct = blob.boundingRect;
		rct.scale(resizeRate);
		rct.setPosition(rct.x*resizeRate, rct.y*resizeRate);

		for (int x = 0; x < rct.width; x++) {
			for (int y = 0; y < rct.height; y++) {
				int px = nChannels*(vidWidth*(rct.y + y) + rct.x + x);

				PixelColor pxc(pixels[px], pixels[px + 1], pixels[px + 2]);
				pxc.ToHSV();
				pxc.SetH(hue);
				pxc.ToRGB();

				//imgOrig.getPixelsRef()[px] = pxc.GetR();
				//imgOrig.getPixelsRef()[px + 1] = pxc.GetG();
				//imgOrig.getPixelsRef()[px + 2] = pxc.GetB();

				origRef[px] = pxc.GetR();
				origRef[px + 1] = pxc.GetG();
				origRef[px + 2] = pxc.GetB();
			}
		}
	}
	imgOrig.setFromPixels(origRef);
}
