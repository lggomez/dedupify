#pragma once
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <ImageMagick\include\Magick++.h>
#include <ImageMagick\include\Magick++\STL.h>

class Example1
{
public:
	Example1();
	~Example1();
	void Example1::GetImagePaths(const std::string & dir_path, std::vector<boost::filesystem::path>& filePaths);
};

