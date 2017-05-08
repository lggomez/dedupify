#pragma once
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <ImageMagick\include\Magick++.h>
#include <ImageMagick\include\Magick++\STL.h>
#include <ImageMagick\include\MagickCore\MagickCore.h>
#include <ImageMagick\include\MagickWand\wand.h>

class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();
	std::map<std::string, char*>* ImageProcessor::ReduceToHash(const std::string currentPath, const std::vector<boost::filesystem::path>* filePaths);
};

