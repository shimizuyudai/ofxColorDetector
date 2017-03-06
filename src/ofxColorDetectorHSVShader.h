#pragma once
#include "ofMain.h"
#include "opencv2/opencv.hpp"
#include "ofxOpenCv.h"
#include "ofxJSONElement.h"
#include <omp.h>

class ofxColorDetectorHSVShader :ofThread
{
public:
	void update(const ofTexture& texture);
	void setTargetColor(const ofTexture& texture, int x, int y);
	void setTargetColor(int h, int s, int b);
	void setHueDistanceThreshold(float value);
	void setSaturationDistanceThreshold(float value);
	void setBrightnessDistanceThreshold(float value);
	void setup(int w, int h, string flagShaderFileName);
	void setMinArea(int value);
	void setMaxArea(int value);
	void setNConsidered(int value);
	void loadSettings(string fileName);
	void exportSettings(string fileName);
	const vector<ofxCvBlob>& getBlobs();
	const ofxCvGrayscaleImage& getMaskImage();
	ofxCvColorImage hsvImg;
	int h, s, b;
	ofFbo fbo;

private:
	ofxCvContourFinder contourFinder;

	ofxCvGrayscaleImage mask;
	ofxCvColorImage colorMask;
	int minArea, maxArea, nConsidered;
	
	float distanceThreshold;
	float hDistThreshold, sDistThreshold,  bDistThreshold;
	ofShader shader;
	
};