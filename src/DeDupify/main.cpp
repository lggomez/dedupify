// WinTestProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../FileSystem/FileSystem.h"
#include "../ImageProcessor/ImageProcessor.h"

int main(int argc, char* argv[])
{
	FileSystem ex1;
	std::vector<boost::filesystem::path> paths;
	std::map<std::string, char*> imageHashes;
	ex1.GetImagePaths("c:\\imgtest", &paths);

	std::cout << "Found " << paths.size() << " elements" << std::endl;

	ImageProcessor im1;
	im1.ReduceToHash(argv[0], &paths, &imageHashes);
	std::cout << "Finished reducing images" << std::endl;

	std::string str;
	std::getline(std::cin, str);
	return 0;
}
