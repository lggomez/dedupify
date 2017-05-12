#pragma once
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <ImageMagick/include/Magick++.h>

class FileSystem
{
public:
	FileSystem();
	~FileSystem();
	void FileSystem::GetImagePaths(const std::string & dir_path, std::vector<boost::filesystem::path>* filePaths);
};

