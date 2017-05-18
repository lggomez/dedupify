#pragma once
#include <iostream>
#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <../DeDupify/ThrowAssert.hpp>
#include <../DeDupify/constants.hpp>

using namespace std;

struct ImageMagnitudeData {
	string filePath;
	double_t distance;
	double_t magnitudeMedian;
	double_t* imageMagnitudes;
};

class __declspec(dllexport) ImageIndexer
{
public:
	ImageIndexer();
	~ImageIndexer();
	static vector<vector<pair<string, char*>>> ImageIndexer::CreateIndex(map<string, char*> imageHashes);
	static vector<vector<ImageMagnitudeData>> ImageIndexer::CreateRankDFTIndex(map<string, pair<double_t, double_t*>>& imageMagnitudes);
};

