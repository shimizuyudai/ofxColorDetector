#pragma once
#include "ofMain.h"
#include "opencv2/opencv.hpp"
#include "ofxOpenCv.h"
#include "ofxJSONElement.h"

class ofxColorDetectorHSV
{
public:
	void update(const ofTexture& texture);
	void setTargetColor(const ofTexture& texture, int x, int y);
	void setTargetColor(int h, int s, int b);
	void setHueDistanceThreshold(int value);
	void setSaturationDistanceThreshold(int value);
	void setBrightnessDistanceThreshold(int value);
	void setup(int w, int h);
	void setMinArea(int value);
	void setMaxArea(int value);
	void setNConsidered(int value);
	void loadSettings(string fileName);
	void exportSettings(string fileName);
	const vector<ofxCvBlob>& getBlobs();
	const ofxCvGrayscaleImage& getMaskImage();
	

private:
	ofxCvContourFinder contourFinder;
	ofxCvColorImage hsvImg;
	ofxCvGrayscaleImage mask;
	int minArea, maxArea, nConsidered;
	int h, s, b;
	float distanceThreshold;
	int hDistThreshold;
	int sDistThreshold;
	int bDistThreshold;
};