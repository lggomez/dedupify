#include "stdafx.h"
#include <iomanip>
#include "../FileSystem/FileSystem.h"
#include "../ImageProcessor/ImageProcessor.h"
#include "../ImageIndexer/ImageIndexer.h"

void FindMatchesWithQuantizationComparer(char* argv[], vector<boost::filesystem::path> paths) {
	map<string, char*> imageHashes;

	// Hash creation
	ImageProcessor imageProcessor;
	imageProcessor.ReduceToHash(argv[0], paths, imageHashes);
	cout << endl << "Finished reducing images, " << imageHashes.size() << " elements processed" << endl << endl;

#if _DEBUG
	std::cout << "Computed hashes:" << std::endl;
	for (auto const& x : imageHashes)
	{
		std::cout << "\t" << x.second << "; " << ((boost::filesystem::path)(x.first)).filename() << std::endl;
	}
#endif

	// Hash indexing
	cout << endl << "Generating image index:" << endl;
	ImageIndexer imageIndexer;
	vector<vector<pair<string, char*>>> imageIndex = imageIndexer.CreateIndex(imageHashes);

	cout << "\tListing element(s):" << endl;
	for (auto const& imageIndexElement : imageIndex)
	{
		if (imageIndexElement.size() > 1) {
			cout << "\t\t-Image similarity found:" << endl;

			for (auto const& imageIndexKey : imageIndexElement)
			{
				cout << "\t\t" << imageIndexKey.second << " - " << imageIndexKey.first << endl;
			}
		}
	}
}

void FindMatchesWithRankDFTComparer(char* argv[], vector<boost::filesystem::path> paths) {
	map<string, pair<double_t, double_t*>> imageHashes;

	// Magnitudes creation
	ImageProcessor imageProcessor;
	imageProcessor.ReduceWithDFT(argv[0], paths, imageHashes);
	cout << endl << "Finished reducing images, " << imageHashes.size() << " elements processed" << endl << endl;

	// Magnitude indexing
	cout << endl << "Generating image index:" << endl;
	ImageIndexer imageIndexer;
	vector<vector<ImageMagnitudeData>> imageIndex = imageIndexer.CreateRankDFTIndex(imageHashes);

	cout << "\tListing element(s):" << endl;
	for (vector<ImageMagnitudeData>& imageIndexElement : imageIndex)
	{
		if (imageIndexElement.size() > 1) {
			cout << "\t\t-Image similarity found:" << endl;

			for (ImageMagnitudeData& imageIndexKey : imageIndexElement)
			{
				cout << fixed << showpoint;
				cout << setprecision(20);
				cout << "\t\t" << imageIndexKey.distance << " - " << imageIndexKey.filePath << endl;
			}
		}
	}
}


vector<boost::filesystem::path> init(int argc, char* argv[]) {
	throw_assert( argc > 1, "Invalid argument count");
	char* filePath = argv[1];

	// Initialization
	vector<boost::filesystem::path> paths;


	// Path retrieval
	FileSystem fileSystem;
	fileSystem.GetImagePaths(filePath, paths);
	cout << "Found " << paths.size() << " elements. Processing" << endl;
	
	return paths;
}

void end() {
	cout << endl << "Finished. Press enter to exit" << endl;
	getchar();
}

int main(int argc, char* argv[])
{
	vector<boost::filesystem::path> paths = init(argc, argv);

	FindMatchesWithRankDFTComparer(argv, paths);

	end();
	return 0;
}

