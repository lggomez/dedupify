#pragma once
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <ImageMagick/include/Magick++.h>

using namespace std;

struct __declspec(dllexport) DftImageData {
	string filePath;
	double_t distance;
	double_t magnitudeMedian;
	double_t* imageMagnitudes;
	Magick::Color* phaseColors;
	Magick::Color* magColors;
	double_t frequencyMedian;
};