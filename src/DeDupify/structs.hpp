#pragma once
#include <ImageMagick/include/Magick++.h>

struct __declspec(dllexport) DftImageData {
	std::string filePath;
	double_t distance;

	double_t magnitudeMedian;
	double_t frequencyMedian;

	double_t* magnitudeQuantums;
	double_t* phaseQuantums;

	Magick::Color* magnitudeColors;
	Magick::Color* phaseColors;
};