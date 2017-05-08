#pragma once
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <ImageMagick\include\Magick++.h>
#include <ImageMagick\include\Magick++\STL.h>
#include <boost/system/config.hpp>

class FileSystem
{
public:
	FileSystem();
	~FileSystem();
	void FileSystem::GetImagePaths(const std::string & dir_path, std::vector<boost::filesystem::path>& filePaths);
};

