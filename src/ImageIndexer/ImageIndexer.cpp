#include "stdafx.h"
#include "ImageIndexer.h"

using namespace std;
using namespace boost::assign;
using namespace Magick;

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

double_t RankDFT(DftImageData a, DftImageData b, size_t n) {
	double_t subresult_a = 0.0;
	double_t subresult_b = 0.0;
	double_t subresult_c = 0.0;

	for (size_t i = 0; i < n; i++)
	{
		Color phaseColora = a.magnitudeColors[i];
		Color phaseColorb = b.magnitudeColors[i];
		double_t aSquaredModule = pow(phaseColora.quantumRed(), 2) + pow(phaseColora.quantumGreen(), 2) + pow(phaseColora.quantumBlue(), 2);
		double_t bSquaredModule = pow(phaseColorb.quantumRed(), 2) + pow(phaseColorb.quantumGreen(), 2) + pow(phaseColorb.quantumBlue(), 2);

		subresult_a += a.magnitudeQuantums[i] * b.magnitudeQuantums[i] - n * (a.frequencyMedian * b.frequencyMedian);
		subresult_b += aSquaredModule - n * pow(a.frequencyMedian, 2);
		subresult_c += bSquaredModule - n * pow(b.frequencyMedian, 2);
	}

	double_t result = pow(subresult_a, 2) / (subresult_b * subresult_c);

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

vector<vector<DftImageData>> ImageIndexer::CreateRankDFTIndex(map<string, DftImageData>& imageDftData) {
	vector<vector<DftImageData>> imageIndex;

	for (auto const& imageMagnitudeData : imageDftData)
	{
		DftImageData imageData;
		imageData.distance = 0.0;
		imageData.filePath = imageMagnitudeData.first;
		imageData.magnitudeMedian = imageMagnitudeData.second.magnitudeMedian;
		imageData.frequencyMedian = imageMagnitudeData.second.frequencyMedian;
		imageData.magnitudeQuantums = imageMagnitudeData.second.magnitudeQuantums;
		imageData.phaseColors = imageMagnitudeData.second.phaseColors;
		imageData.magnitudeColors = imageMagnitudeData.second.magnitudeColors;
		imageData.phaseQuantums = imageMagnitudeData.second.phaseQuantums;

		if (imageIndex.size() == 0) {
			// Initialize index with the first hash
			vector<DftImageData> imageIndexKeyList;
			imageData.distance = 1;
			imageIndexKeyList.push_back(imageData);
			imageIndex.push_back(imageIndexKeyList);

			continue;
		}

		// Traverse the index in search of matches in each indexKey lists
		bool match = false;
		for (vector<DftImageData>& imageIndexElement : imageIndex)
		{
			// Traverse each index key from the index element
			for (DftImageData& subImageData : imageIndexElement)
			{
				imageData.distance = RankDFT(imageData, subImageData, DFT_IMAGE_HEIGHT * DFT_IMAGE_WIDTH);
				if (imageData.distance/* > 1.00000000000*/) { //TODO TEST: Add a criteria after verifying the algorithm
					// There is a match, so we add the current match to the index element
					match = true;
					imageIndexElement.push_back(imageData);
					break;
				}
			}
		}

		if (!match) {
			// No matches, add the key into its separate index element
			vector<DftImageData> imageIndexKeyList;
			imageIndexKeyList.push_back(imageData);
			imageIndex.push_back(imageIndexKeyList);
		}
	}

	return imageIndex;
}