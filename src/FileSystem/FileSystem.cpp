#include "stdafx.h"
#include "FileSystem.h"

using namespace std;
namespace fs = boost::filesystem;

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

void FileSystem::GetImagePaths(const std::string & dir_path, std::vector<boost::filesystem::path>& filePaths)
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

	formatIndex.erase("GIF");
	formatIndex.erase("GIF87");
	formatIndex.erase("TXT");

	if (!fs::exists(dir_path)) return;
	fs::recursive_directory_iterator end_itr;
	for (fs::recursive_directory_iterator itr(dir_path);
		itr != end_itr;
		++itr)
	{
		if (!is_directory(*itr))
		{
			std::string fileExt = itr->path().extension().string();
			boost::erase_all(fileExt, ".");
			boost::to_upper(fileExt);

			if (formatIndex.find(fileExt) != formatIndex.end()) {
#if _DEBUG
				const boost::filesystem::path& filePath = itr->path();
				std::string filePathString = filePath.string();
				std::wstring filePathwString = filePath.wstring();
				filePaths.push_back(filePath);
#else
				filePaths.push_back(itr->path());
#endif //_DEBUG

			}
		}
	}

	(void)MagickCore::DestroyExceptionInfo(exceptionInfo);
}