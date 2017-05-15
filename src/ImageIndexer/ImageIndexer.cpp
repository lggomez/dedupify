#include "stdafx.h"
#include "ImageIndexer.h"

using namespace std;
using namespace boost::assign;

ImageIndexer::ImageIndexer()
{
}

ImageIndexer::~ImageIndexer()
{
}

bool HashesAreSimilar(const char* a, const char* b, const size_t size, const size_t threshold) {
	throw_assert((threshold <= size), "Invalid threshold, it surprasses the hash size");
	unsigned __int64 returnValue = 1ULL;

	for (size_t i = 0; i < size; ++i)
	{
		if (a[i] - b[i] != 0) {
			returnValue <<= 1;
		}
	}

	return (1ULL << threshold) >= returnValue;
}

double NarayananDistance(ImageMagnitudeData a, ImageMagnitudeData b, size_t n) {
	double result = 0.0;
	double subresult_a = 0.0;
	double subresult_b = 0.0;
	double subresult_c = 0.0;

	for (size_t i = 0; i < n; i++)
	{
		subresult_a += (a.imageMagnitudes[i] * b.imageMagnitudes[i]) - (n * (a.magnitudeMedian * b.magnitudeMedian));
		subresult_c += (abs(a.imageMagnitudes[i]) * abs(a.imageMagnitudes[i])) - (n * (a.magnitudeMedian * a.magnitudeMedian));
		subresult_c += (abs(b.imageMagnitudes[i]) * abs(b.imageMagnitudes[i])) - (n * (b.magnitudeMedian * b.magnitudeMedian));
	}

	result = (subresult_a * subresult_a) / (subresult_b * subresult_c);

	return result;
}

std::vector<vector<pair<std::string, char*>>> ImageIndexer::CreateIndex(std::map<std::string, char*> imageHashes) {
	std::vector<vector<pair<std::string, char*>>> imageIndex;

	for (auto const& imageHash : imageHashes)
	{
		pair<std::string, char*> imageIndexKey = make_pair(imageHash.first, imageHash.second);

		if (imageIndex.size() == 0) { 
			// Initialize index with the first hash
			std::vector<pair<std::string, char*>> imageIndexKeyList;
			imageIndexKeyList.push_back(imageIndexKey);
			imageIndex.push_back(imageIndexKeyList);

			continue;
		}

		// Traverse the index in search of matches in each indexKey lists
		bool match = false;
		for (vector<pair<std::string, char*>>& imageIndexElement : imageIndex)
		{
			// Traverse each index key from the index element
			for (pair<std::string, char*> indexKey : imageIndexElement)
			{
				if (HashesAreSimilar(indexKey.second, imageIndexKey.second, HASH_SIZE, HASH_THRESHOLD)) {
					// There is a match, so we add the current match to the index element
					match = true;
					imageIndexElement.push_back(imageIndexKey);
					break;
				}
			}
		}

		if (!match) {
			// No matches, add the key into its separate index element
			std::vector<pair<std::string, char*>> imageIndexKeyList;
			imageIndexKeyList.push_back(imageIndexKey);
			imageIndex.push_back(imageIndexKeyList);
		}
	}

	return imageIndex;
}

std::vector<std::vector<ImageMagnitudeData>> ImageIndexer::CreateNarayananDFTIndex(std::map<std::string, std::pair<double, unsigned short*>>& imageMagnitudes) {
	std::vector<std::vector<ImageMagnitudeData>> imageIndex;

	for (auto const& imageMagnitudeData : imageMagnitudes)
	{
		ImageMagnitudeData imageData;
		imageData.distance = 0.0;
		imageData.filePath = imageMagnitudeData.first;
		imageData.magnitudeMedian = imageMagnitudeData.second.first;
		imageData.imageMagnitudes = imageMagnitudeData.second.second;

		if (imageIndex.size() == 0) {
			// Initialize index with the first hash
			std::vector<ImageMagnitudeData> imageIndexKeyList;
			imageIndexKeyList.push_back(imageData);
			imageIndex.push_back(imageIndexKeyList);

			continue;
		}

		// Traverse the index in search of matches in each indexKey lists
		bool match = false;
		for (std::vector<ImageMagnitudeData>& imageIndexElement : imageIndex)
		{
			// Traverse each index key from the index element
			for (ImageMagnitudeData imageMagnitudeData : imageIndexElement)
			{
				if (NarayananDistance(imageData, imageMagnitudeData, DFT_IMAGE_SIZE) > 0.9) {
					// There is a match, so we add the current match to the index element
					match = true;
					imageIndexElement.push_back(imageMagnitudeData);
					break;
				}
			}
		}

		if (!match) {
			// No matches, add the key into its separate index element
			std::vector<ImageMagnitudeData> imageIndexKeyList;
			imageIndexKeyList.push_back(imageData);
			imageIndex.push_back(imageIndexKeyList);
		}
	}

	return imageIndex;
}