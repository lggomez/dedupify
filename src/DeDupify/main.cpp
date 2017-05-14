#include "stdafx.h"
#include "../FileSystem/FileSystem.h"
#include "../ImageProcessor/ImageProcessor.h"
#include "../ImageIndexer/ImageIndexer.h"

int main(int argc, char* argv[])
{
	throw_assert((argc > 1), "Invalid argument count");
	char* filePath = argv[1];

	// Initialization
	std::vector<boost::filesystem::path> paths;
	std::map<std::string, char*> imageHashes;

	// Path retrieval
	FileSystem fileSystem;
	fileSystem.GetImagePaths(filePath, &paths);
	std::cout << "Found " << paths.size() << " elements. Processing" << std::endl;

	// Hash creation
	ImageProcessor imageProcessor;
	imageProcessor.ReduceToHash(argv[0], &paths, &imageHashes);
	std::cout << std::endl << "Finished reducing images, " << imageHashes.size() << " elementsm processed" << std::endl << std::endl;

#if _DEBUG
	std::cout << "Computed hashes:" << std::endl;
	for (auto const& x : imageHashes)
	{
		std::cout << "\t" << x.second << "; " << ((boost::filesystem::path)(x.first)).filename() << std::endl;
	}
#endif

	// Hash indexing
	std::cout << std::endl << "Generating image index:" << std::endl;
	ImageIndexer imageIndexer;
	std::vector<vector<pair<std::string, char*>>> imageIndex = imageIndexer.CreateIndex(imageHashes);

	std::cout << "\tListing element(s):" << std::endl;
	for (auto const& imageIndexElement : imageIndex)
	{
		if (imageIndexElement.size() > 1) {
			std::cout << "\t\t-Image similarity found:" << std::endl;

			for (auto const& imageIndexKey : imageIndexElement)
			{
				std::cout << "\t\t" << imageIndexKey.second << " - " << imageIndexKey.first << std::endl;
			}
		}
	}

	std::cout << std::endl << "Finished. Press enter to exit" << std::endl;
	std::getchar();
	return 0;
}

