#pragma once
#include <codecvt>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <ImageMagick\include\Magick++.h>
#include <ImageMagick\include\Magick++\STL.h>
#include <ImageMagick\include\MagickCore\MagickCore.h>
#include <../DeDupify/constants.hpp>

class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();
	void ImageProcessor::ReduceToHash(const std::string& currentPath, const std::vector<boost::filesystem::path>& filePaths, std::map<std::string, char*>& imageHashes);
	void ImageProcessor::ReduceWithDFT(const std::string& currentPath, const std::vector<boost::filesystem::path>& filePaths, std::map<std::string, std::pair<double, unsigned short*>>& imageMagnitudes);
};

