#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <codecvt>
#include <iostream>
#include <vector>
#include <complex>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <ImageMagick/include/Magick++.h>
#include <ImageMagick/include/Magick++/STL.h>
#include <ImageMagick/include/MagickCore/MagickCore.h>
#include <../External/fttw/include/fftw3.h>
#include <../DeDupify/constants.hpp>
#include <../DeDupify/structs.hpp>

class __declspec(dllexport) ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();
	static void ImageProcessor::ReduceToHash(const string& currentPath, const vector<boost::filesystem::path>& filePaths, map<string, char*>& imageHashes);
	static void ImageProcessor::ReduceWithDFT(const string& currentPath, const vector<boost::filesystem::path>& filePaths,  map<string, DftImageData>& imageDftData);
};

