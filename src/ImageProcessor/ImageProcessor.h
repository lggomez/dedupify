#pragma once
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <ImageMagick\include\Magick++.h>
#include <ImageMagick\include\Magick++\STL.h>
#include <../DeDupify/ThrowAssert.hpp>

class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();
	void ImageProcessor::ReduceToHash(const std::string currentPath, const std::vector<boost::filesystem::path>* filePaths, std::map<std::string, char*>* imageHashes);
};

