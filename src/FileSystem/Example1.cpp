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
		boost::to_upper(formats[i]);
		formatIndex.insert(std::pair<std::string, bool>(std::string(formats[i]), true));
		++i;
	}

	if (!fs::exists(dir_path)) return;
	fs::recursive_directory_iterator end_itr;
	for (fs::recursive_directory_iterator itr(dir_path);
		itr != end_itr;
		++itr)
	{
		if (!is_directory(*itr))
		{
			cout << itr->path() << std::endl;
			std::string filePath = itr->path().extension().string();
			boost::erase_all(filePath, ".");
			boost::to_upper(filePath);

			if (formatIndex.find(filePath) != formatIndex.end()) {
				filePaths.push_back(itr->path());
				//break;
			}
		}
	}

	(void)MagickCore::DestroyExceptionInfo(exceptionInfo);
}