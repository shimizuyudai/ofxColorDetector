#include "ofxColorDetector.h"

void ofxColorDetector::update(const ofTexture & texture)
{
	ofPixels pixels;
	texture.readToPixels(pixels);
	auto& maskPixels = mask.getPixels();
	for (int i = 0; i < texture.getWidth()*texture.getHeight(); i++) {
		auto red = pixels[i*3];
		auto green = pixels[i*3+1];
		auto blue = pixels[i*3+2];
		if (ofDist(red,green,blue,targetColor.r,targetColor.g,targetColor.b) < distanceThreshold) {
			maskPixels[i] = 255;
		}
		else {
			maskPixels[i] = 0;
		}
	}
	mask.flagImageChanged();
	contourFinder.findContours(mask,minArea,maxArea, nConsidered,false);
	cout << contourFinder.nBlobs << endl;
}

void ofxColorDetector::setTargetColor(const ofTexture & texture, int x, int y)
{
	if (x < 0 || y < 0 || x >= texture.getWidth() || y >= texture.getWidth()) {
		return;
	}
	ofPixels pixels;
	texture.readToPixels(pixels);
	if (pixels.size() < texture.getWidth()*texture.getHeight()* 3) {
		cout << "error" << endl;
		return;
	}
	auto i = ((x + y*texture.getWidth()) * 3);
	auto r = pixels[i];
	auto g = pixels[i + 1];
	auto b = pixels[i + 2];
	setTargetColor(r,g,b);

}

void ofxColorDetector::setTargetColor(int r, int g, int b)
{
	targetColor.r = r;
	targetColor.g = g;
	targetColor.b = b;
}

void ofxColorDetector::setDistanceThreshold(float value)
{
	this->distanceThreshold = value;
}

void ofxColorDetector::setup(int w, int h)
{
	mask.allocate(w,h);
}

void ofxColorDetector::setMinArea(int value)
{
	this->minArea = value;
}

void ofxColorDetector::setMaxArea(int value)
{
	this->maxArea = value;
}

void ofxColorDetector::setNConsidered(int value)
{
	this->nConsidered = value;
}

void ofxColorDetector::loadSettings(string fileName)
{
	ofxJSONElement json;
	auto result = json.open(fileName);
	if (result) {
		auto r = json["targetColor"]["r"].asInt();
		auto g = json["targetColor"]["g"].asInt();
		auto b = json["targetColor"]["b"].asInt();
		targetColor = ofColor(r, g, b);

		minArea = json["minArea"].asInt();
		maxArea = json["maxArea"].asInt();
		nConsidered = json["nConsidered"].asInt();

		distanceThreshold = json["distanceThreshold"].asFloat();
	}
}

void ofxColorDetector::exportSettings(string fileName)
{
	ofxJSONElement json;
	json["targetColor"]["r"] = targetColor.r;
	json["targetColor"]["g"] = targetColor.g;
	json["targetColor"]["b"] = targetColor.b;
	json["minArea"] = minArea;
	json["maxArea"] = maxArea;
	json["nConsidered"] = nConsidered;
	json["distanceThreshold"] = distanceThreshold;
	json.save(fileName);
}

const vector<ofxCvBlob>& ofxColorDetector::getBlobs()
{
	return contourFinder.blobs;
}

const ofxCvGrayscaleImage & ofxColorDetector::getMaskImage()
{
	return this->mask;
}
