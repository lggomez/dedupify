#include "stdafx.h"
#include <iostream>
#include <vector>
#include "Example1.h"

using namespace std;
namespace fs = boost::filesystem;

Example1::Example1()
{
}

Example1::~Example1()
{
}

std::vector<boost::filesystem::path>& Example1::GetImagePaths(std::string & dir_path)
{
	std::vector<boost::filesystem::path> imagePaths;

	MagickCore::ExceptionInfo* exceptionInfo = MagickCore::AcquireExceptionInfo();
	size_t number_formats;
	char** formats = MagickCore::GetMagickList("*", &number_formats, exceptionInfo);

	fs::directory_iterator iterator(dir_path);
	for (; iterator != fs::directory_iterator(); ++iterator)
	{
		int i = 0;
		while (i < number_formats) {
			if (iterator->path().filename().extension().compare(std::string(formats[i])) == 0) {
				imagePaths.push_back(iterator->path());
				cout << (iterator->path().filename()) << endl;
				break;
			}
			i++;
		}
	}

	(void)MagickCore::DestroyExceptionInfo(exceptionInfo);

	return imagePaths;
}