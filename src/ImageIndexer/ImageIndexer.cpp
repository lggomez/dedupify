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
	throw_assert(threshold <= size, "Invalid threshold, it surprasses the hash size");
	unsigned __int64 returnValue = 1ULL;

	for (size_t i = 0; i < size; ++i)
	{
		if (a[i] - b[i] != 0) {
			returnValue <<= 1;
		}
	}

	return 1ULL << threshold >= returnValue;
}

double_t RankDFT(ImageMagnitudeData a, ImageMagnitudeData b, size_t n) {
	double_t subresult_a = 0.0;
	double_t subresult_b = 0.0;
	double_t subresult_c = 0.0;

	for (size_t i = 0; i < n; i++)
	{
		subresult_a += a.imageMagnitudes[i] * b.imageMagnitudes[i] - n * (a.magnitudeMedian * b.magnitudeMedian);
		subresult_b += abs(a.imageMagnitudes[i]) * abs(a.imageMagnitudes[i]) - n * (a.magnitudeMedian * a.magnitudeMedian);
		subresult_c += abs(b.imageMagnitudes[i]) * abs(b.imageMagnitudes[i]) - n * (b.magnitudeMedian * b.magnitudeMedian);
	}

	double_t result = subresult_a * subresult_a / (subresult_b * subresult_c);

	return result;
}

vector<vector<pair<string, char*>>> ImageIndexer::CreateIndex(map<string, char*> imageHashes) {
	vector<vector<pair<string, char*>>> imageIndex;

	for (auto const& imageHash : imageHashes)
	{
		pair<string, char*> imageIndexKey = make_pair(imageHash.first, imageHash.second);

		if (imageIndex.size() == 0) { 
			// Initialize index with the first hash
			vector<pair<string, char*>> imageIndexKeyList;
			imageIndexKeyList.push_back(imageIndexKey);
			imageIndex.push_back(imageIndexKeyList);

			continue;
		}

		// Traverse the index in search of matches in each indexKey lists
		bool match = false;
		for (vector<pair<string, char*>>& imageIndexElement : imageIndex)
		{
			// Traverse each index key from the index element
			for (pair<string, char*> indexKey : imageIndexElement)
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
			vector<pair<string, char*>> imageIndexKeyList;
			imageIndexKeyList.push_back(imageIndexKey);
			imageIndex.push_back(imageIndexKeyList);
		}
	}

	return imageIndex;
}

vector<vector<ImageMagnitudeData>> ImageIndexer::CreateRankDFTIndex(map<string, pair<double_t, double_t*>>& imageMagnitudes) {
	vector<vector<ImageMagnitudeData>> imageIndex;

	for (auto const& imageMagnitudeData : imageMagnitudes)
	{
		ImageMagnitudeData imageData;
		imageData.distance = 0.0;
		imageData.filePath = imageMagnitudeData.first;
		imageData.magnitudeMedian = imageMagnitudeData.second.first;
		imageData.imageMagnitudes = imageMagnitudeData.second.second;

		if (imageIndex.size() == 0) {
			// Initialize index with the first hash
			vector<ImageMagnitudeData> imageIndexKeyList;
			imageData.distance = 1;
			imageIndexKeyList.push_back(imageData);
			imageIndex.push_back(imageIndexKeyList);

			continue;
		}

		// Traverse the index in search of matches in each indexKey lists
		bool match = false;
		for (vector<ImageMagnitudeData>& imageIndexElement : imageIndex)
		{
			// Traverse each index key from the index element
			for (ImageMagnitudeData& subImageData : imageIndexElement)
			{
				imageData.distance = RankDFT(imageData, subImageData, DFT_IMAGE_HEIGHT * DFT_IMAGE_WIDTH);
				if (imageData.distance > 0.9) {
					// There is a match, so we add the current match to the index element
					match = true;
					imageIndexElement.push_back(imageData);
					break;
				}
			}
		}

		if (!match) {
			// No matches, add the key into its separate index element
			vector<ImageMagnitudeData> imageIndexKeyList;
			imageIndexKeyList.push_back(imageData);
			imageIndex.push_back(imageIndexKeyList);
		}
	}

	return imageIndex;
}