#pragma once
#include "ofMain.h"
#include "opencv2/opencv.hpp"
#include "ofxOpenCv.h"
#include "ofxJSONElement.h"

class ofxColorDetector
{
public:
	void update(const ofTexture& texture);
	void setTargetColor(const ofTexture& texture,int x,int y);
	void setTargetColor(int r,int g,int b);
	void setDistanceThreshold(float value);
	void setup(int w,int h);
	void setMinArea(int value);
	void setMaxArea(int value);
	void setNConsidered(int value);
	void loadSettings(string fileName);
	void exportSettings(string fileName);
	const vector<ofxCvBlob>& getBlobs();
	const ofxCvGrayscaleImage& getMaskImage();

private:
	ofColor targetColor;
	ofxCvGrayscaleImage mask;
	ofxCvContourFinder contourFinder;
	int minArea, maxArea, nConsidered;
	float distanceThreshold;
};