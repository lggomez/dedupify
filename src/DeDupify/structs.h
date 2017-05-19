#pragma once
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <ImageMagick/include/Magick++.h>

using namespace std;

struct ImageData {
	string filePath;
	double_t distance;
	double_t magnitudeMedian;
	double_t* imageMagnitudes;
	Magick::Color* phaseColors;
	double_t frequencyMedian;
};

struct DftImageData {
	double_t* imageMagnitudes;
	Magick::Color* phaseColors;
	double_t magnitudeMedian;
	double_t frequencyMedian;
};