#pragma once
#include <iostream>
#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <../DeDupify/ThrowAssert.hpp>
#include <../DeDupify/constants.hpp>

using namespace std;

struct ImageMagnitudeData {
	std::string filePath;
	double distance;
	double magnitudeMedian;
	unsigned short* imageMagnitudes;
};

class ImageIndexer
{
public:
	ImageIndexer();
	~ImageIndexer();
	std::vector<vector<pair<std::string, char*>>> ImageIndexer::CreateIndex(std::map<std::string, char*> imageHashes);
	std::vector<std::vector<ImageMagnitudeData>> ImageIndexer::CreateNarayananDFTIndex(std::map<std::string, std::pair<double, unsigned short*>>& imageMagnitudes);
};

