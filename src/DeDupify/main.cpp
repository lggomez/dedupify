#include "stdafx.h"
#include "../FileSystem/FileSystem.h"
#include "../ImageProcessor/ImageProcessor.h"
#include "../ImageIndexer/ImageIndexer.h"

int main(int argc, char* argv[])
{
	FileSystem ex1;
	std::vector<boost::filesystem::path> paths;
	std::map<std::string, char*> imageHashes;

	//Path retrieval
	ex1.GetImagePaths("c:\\imgtest", &paths);
	std::cout << "Found " << paths.size() << " elements" << std::endl;

	//Hash creation
	ImageProcessor im1;
	im1.ReduceToHash(argv[0], &paths, &imageHashes);
	std::cout << std::endl << "Finished reducing images, processed " << imageHashes.size() << " elements" << std::endl << std::endl;

	std::cout << "Computed hashes:" << std::endl;
	for (auto const& x : imageHashes)
	{
		std::cout << "\t" << x.second << "; " << ((boost::filesystem::path)(x.first)).filename() << std::endl;
	}

	//Hash indexing
	std::cout << std::endl << "Generating image index:" << std::endl;
	//ImageIndexer ii1;
	//ii1.CreateIndex(imageHashes);

	std::string str;
	std::getline(std::cin, str);
	return 0;
}

