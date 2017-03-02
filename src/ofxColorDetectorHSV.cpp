#include "ofxColorDetectorHSV.h"

void ofxColorDetectorHSV::update(const ofTexture & texture)
{
	ofPixels pixels;
	texture.readToPixels(pixels);
	hsvImg.setFromPixels(pixels);
	hsvImg.convertRgbToHsv();
	ofPixels hsvPixels;
	hsvPixels = hsvImg.getPixels();
	auto& maskPixels = mask.getPixels();
	for (int i = 0; i < texture.getWidth()*texture.getHeight(); i++) {
		auto hue = hsvPixels[i * 3];
		auto saturation = hsvPixels[i * 3 + 1];
		auto brightness = hsvPixels[i * 3 + 2];
		if (abs(h - hue) < hDistThreshold && abs(s - saturation) < sDistThreshold && abs(b - brightness) < bDistThreshold) {
			maskPixels[i] = 255;
		}
		else {
			maskPixels[i] = 0;
		}
	}
	mask.flagImageChanged();
	contourFinder.findContours(mask, minArea, maxArea, nConsidered, false);
}

void ofxColorDetectorHSV::setTargetColor(const ofTexture & texture, int x, int y)
{
	if (x < 0 || y < 0 || x >= texture.getWidth() || y >= texture.getWidth()) {
		return;
	}
	ofPixels pixels;
	texture.readToPixels(pixels);
	hsvImg.setFromPixels(pixels);
	hsvImg.convertRgbToHsv();
	ofPixels hsvPixels = hsvImg.getPixels();
	if (hsvPixels.size() < texture.getWidth()*texture.getHeight() * 3) {
		cout << "error" << endl;
		return;
	}
	auto i = ((x + y*texture.getWidth()) * 3);
	auto h = hsvPixels[i];
	auto s = hsvPixels[i + 1];
	auto b = hsvPixels[i + 2];
	setTargetColor(h,s,b);

}

void ofxColorDetectorHSV::setTargetColor(int h, int s, int b)
{
	this->h = h;
	this->s = s;
	this->b = b;
}

void ofxColorDetectorHSV::setHueDistanceThreshold(int value)
{
	hDistThreshold = value;
}

void ofxColorDetectorHSV::setSaturationDistanceThreshold(int value)
{
	sDistThreshold = value;
}

void ofxColorDetectorHSV::setBrightnessDistanceThreshold(int value)
{
	bDistThreshold = value;
}


void ofxColorDetectorHSV::setup(int w, int h)
{
	mask.allocate(w, h);
	hsvImg.allocate(w,h);
}

void ofxColorDetectorHSV::setMinArea(int value)
{
	this->minArea = value;
}

void ofxColorDetectorHSV::setMaxArea(int value)
{
	this->maxArea = value;
}

void ofxColorDetectorHSV::setNConsidered(int value)
{
	this->nConsidered = value;
}

void ofxColorDetectorHSV::loadSettings(string fileName)
{
	ofxJSONElement json;
	auto result = json.open(fileName);
	if (result) {
		h = json["targetColor"]["h"].asInt();
		s = json["targetColor"]["s"].asInt();
		b = json["targetColor"]["b"].asInt();
		
		hDistThreshold = json["hueDistThreshold"].asInt();
		sDistThreshold = json["saturationDistThreshold"].asInt();
		bDistThreshold = json["brightnessDistThreshold"].asInt();

		minArea = json["minArea"].asInt();
		maxArea = json["maxArea"].asInt();
		nConsidered = json["nConsidered"].asInt();
	}
}

void ofxColorDetectorHSV::exportSettings(string fileName)
{
	ofxJSONElement json;
	json["targetColor"]["h"] = h;
	json["targetColor"]["s"] = s;
	json["targetColor"]["b"] = b;
	
	json["hueDistThreshold"] = hDistThreshold;
	json["saturationDistThreshold"] = sDistThreshold;
	json["brightnessDistThreshold"] = bDistThreshold;

	json["minArea"] = minArea;
	json["maxArea"] = maxArea;
	json["nConsidered"] = nConsidered;
}

const vector<ofxCvBlob>& ofxColorDetectorHSV::getBlobs()
{
	return contourFinder.blobs;
}

const ofxCvGrayscaleImage & ofxColorDetectorHSV::getMaskImage()
{
	return this->mask;
}
