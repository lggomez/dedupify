// WinTestProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../FileSystem/Example1.h"


int main()
{
	Example1 ex1;
	std::vector<boost::filesystem::path> paths;
	ex1.GetImagePaths("c:\\", paths);
	return 0;
}

