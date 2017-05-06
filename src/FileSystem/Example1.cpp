#include "stdafx.h"
#include "Example1.h"

using namespace std;
namespace fs = boost::filesystem;

Example1::Example1()
{
}

Example1::~Example1()
{
}

void Example1::GetImagePaths(const std::string & dir_path, std::vector<boost::filesystem::path>& filePaths)
{
	MagickCore::ExceptionInfo* exceptionInfo = MagickCore::AcquireExceptionInfo();
	size_t number_formats;
	char** formats = MagickCore::GetMagickList("*", &number_formats, exceptionInfo);
	map<std::string, bool> formatIndex;

	int i = 0;
	while (i < number_formats) {
		formatIndex.insert(std::pair<std::string, bool>(std::string(formats[i]), true));
		++i;
	}

	fs::directory_iterator iterator(dir_path);
	for (; iterator != fs::directory_iterator(); ++iterator)
	{
		const std::string file = iterator->path().filename().extension().string();
		if (formatIndex.find(file) != formatIndex.end()) {
			filePaths.push_back(iterator->path());
			cout << (iterator->path().filename()) << endl;
			break;
		}
	}

	(void)MagickCore::DestroyExceptionInfo(exceptionInfo);
}