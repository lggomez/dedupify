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
	double_t distance;
	double_t magnitudeMedian;
	double_t* imageMagnitudes;
};

class __declspec(dllexport) ImageIndexer
{
public:
	ImageIndexer();
	~ImageIndexer();
	std::vector<vector<pair<std::string, char*>>> ImageIndexer::CreateIndex(std::map<std::string, char*> imageHashes);
	std::vector<std::vector<ImageMagnitudeData>> ImageIndexer::CreateRankDFTIndex(std::map<std::string, std::pair<double_t, double_t*>>& imageMagnitudes);
};

