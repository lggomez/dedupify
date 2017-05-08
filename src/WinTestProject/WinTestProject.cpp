// WinTestProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../FileSystem/Example1.h"

void ReportImages(std::vector<boost::filesystem::path> paths) {
	std::cout << "Matches:" << std::endl;

	for (boost::filesystem::path p : paths)
	{
		std::cout << p.filename().string() << " " << std::endl;
	}

	std::cout << "Finished" << std::endl;
}

int main(int argc, char* argv[])
{
	Example1 ex1;
	std::vector<boost::filesystem::path> paths;
	ex1.GetImagePaths("c:\\imgtest", paths);

	ReportImages(paths);

	std::string str;
	std::getline(std::cin, str);
	return 0;
}

