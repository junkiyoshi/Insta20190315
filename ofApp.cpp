#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofEnableDepthTest();

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 200, true, true, true);
	this->span = 20;

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofClear(0);
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	string word = "FACT";
	font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word) * 0.5);

	fbo.end();

	ofPixels pixels;
	fbo.readToPixels(pixels);

	for (int x = 0; x < fbo.getWidth(); x += this->span) {

		for (int y = 0; y < fbo.getHeight(); y += this->span) {

			if (pixels.getColor(x, y) != ofColor(0, 0)) {

				this->locations.push_back(glm::vec2(x - ofGetWidth() * 0.5, ofGetHeight() - y - ofGetHeight() * 0.5));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (auto& location : this->locations) {

		for (int z = this->span * -1; z <= this->span * 1; z += this->span) {

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(location.x * 0.003, location.y * 0.003, z * 0.003, ofGetFrameNum() * 0.01), 0, 1, -PI * 0.5, PI * 0.5), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(location.x * 0.003, location.y * 0.003, z * 0.003, ofGetFrameNum() * 0.01), 0, 1, -PI * 0.5, PI * 0.5), glm::vec3(0, 1, 0));
			glm::vec3 box_location = glm::vec4(location, z, 0) * rotation_x * rotation_y;

			ofFill();
			ofSetColor(39);
			ofDrawBox(box_location, this->span - 1);

			ofNoFill();
			ofSetColor(239);
			ofDrawBox(box_location, this->span);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}