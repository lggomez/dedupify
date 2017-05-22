#pragma once
#include <iostream>
#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <ImageMagick/include/Magick++.h>
#include <ImageMagick/include/Magick++/STL.h>
#include <ImageMagick/include/MagickCore/MagickCore.h>
#include <../DeDupify/ThrowAssert.hpp>
#include <../DeDupify/constants.hpp>
#include <../DeDupify/structs.hpp>

using namespace std;

class __declspec(dllexport) ImageIndexer
{
public:
	ImageIndexer();
	~ImageIndexer();
	static vector<vector<pair<string, char*>>> ImageIndexer::CreateIndex(map<string, char*> imageHashes);
	static vector<vector<ImageData>> ImageIndexer::CreateRankDFTIndex(map<string, DftImageData>& imageDftData);
};

