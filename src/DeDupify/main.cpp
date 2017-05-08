// WinTestProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../FileSystem/FileSystem.h"
#include "../ImageProcessor/ImageProcessor.h"

void ReportImages(const std::vector<boost::filesystem::path>* paths) {
	std::cout << "Matches:" << std::endl;

	for (boost::filesystem::path p : *paths)
	{
		std::cout << p.string() << " " << std::endl;
	}
}

int main(int argc, char* argv[])
{
	FileSystem ex1;
	std::vector<boost::filesystem::path> paths;
	ex1.GetImagePaths("c:\\imgtest", &paths);

	ReportImages(&paths);
	std::cout << "Finished reading images" << std::endl;

	ImageProcessor im1;
	im1.ReduceToHash(argv[0], &paths);
	std::cout << "Finished reducing images" << std::endl;

	std::string str;
	std::getline(std::cin, str);
	return 0;
}

