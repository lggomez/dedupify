#pragma once
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

struct ImageMagnitudeData {
	string filePath;
	double_t distance;
	double_t magnitudeMedian;
	double_t* imageMagnitudes;
};