#include "ofxColorDetectorHSVShader.h"
void ofxColorDetectorHSVShader::update(const ofTexture & texture)
{
	ofPixels pixels;
	texture.readToPixels(pixels);
	hsvImg.setFromPixels(pixels);
	hsvImg.convertRgbToHsv();
	fbo.begin();
	shader.begin();
	shader.setUniformTexture("texture", hsvImg.getTexture(),1);
	shader.setUniform1f("targetH", (float)h / 255.0);
	shader.setUniform1f("targetS", (float)s / 255.0);
	shader.setUniform1f("targetB", (float)b / 255.0);
	shader.setUniform1f("hDistThreshold", hDistThreshold);
	shader.setUniform1f("sDistThreshold", sDistThreshold);
	shader.setUniform1f("bDistThreshold", bDistThreshold);
	hsvImg.draw(0,0);
	shader.end();
	fbo.end();
	ofPixels maskPixels;
	fbo.getTexture().readToPixels(maskPixels);
	colorMask.setFromPixels(maskPixels);
	mask = colorMask;
	contourFinder.findContours(mask, minArea, maxArea, nConsidered, false);
}

void ofxColorDetectorHSVShader::setTargetColor(const ofTexture & texture, int x, int y)
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
	setTargetColor(h, s, b);

}

void ofxColorDetectorHSVShader::setTargetColor(int h, int s, int b)
{
	this->h = h;
	this->s = s;
	this->b = b;
}

void ofxColorDetectorHSVShader::setHueDistanceThreshold(float value)
{
	hDistThreshold = value;
}

void ofxColorDetectorHSVShader::setSaturationDistanceThreshold(float value)
{
	sDistThreshold = value;
}

void ofxColorDetectorHSVShader::setBrightnessDistanceThreshold(float value)
{
	bDistThreshold = value;
}


void ofxColorDetectorHSVShader::setup(int w, int h, string flagShaderFileName)
{
	mask.allocate(w, h);
	colorMask.allocate(w,h);
	hsvImg.allocate(w, h);
	fbo.allocate(w,h,GL_RGB);
	shader.setupShaderFromFile(GL_FRAGMENT_SHADER, flagShaderFileName);
	shader.linkProgram();
}

void ofxColorDetectorHSVShader::setMinArea(int value)
{
	this->minArea = value;
}

void ofxColorDetectorHSVShader::setMaxArea(int value)
{
	this->maxArea = value;
}

void ofxColorDetectorHSVShader::setNConsidered(int value)
{
	this->nConsidered = value;
}

void ofxColorDetectorHSVShader::loadSettings(string fileName)
{
	ofxJSONElement json;
	auto result = json.open(fileName);
	if (result) {
		h = json["targetColor"]["h"].asInt();
		s = json["targetColor"]["s"].asInt();
		b = json["targetColor"]["b"].asInt();

		hDistThreshold = json["hueDistThreshold"].asFloat();
		sDistThreshold = json["saturationDistThreshold"].asFloat();
		bDistThreshold = json["brightnessDistThreshold"].asFloat();

		minArea = json["minArea"].asInt();
		maxArea = json["maxArea"].asInt();
		nConsidered = json["nConsidered"].asInt();
	}
}

void ofxColorDetectorHSVShader::exportSettings(string fileName)
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
	json.save(fileName);
}

const vector<ofxCvBlob>& ofxColorDetectorHSVShader::getBlobs()
{
	return contourFinder.blobs;
}

const ofxCvGrayscaleImage & ofxColorDetectorHSVShader::getMaskImage()
{
	return this->mask;
}
