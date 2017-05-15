#include "stdafx.h"
#include "ImageIndexer.h"

using namespace std;
using namespace boost::assign;

const size_t HASH_SIZE = 128;
const size_t HASH_THRESHOLD = 3;

ImageIndexer::ImageIndexer()
{
}

ImageIndexer::~ImageIndexer()
{
}

bool HashesAreSimilar(const char* a, const char* b, const size_t size, const size_t threshold) {
	throw_assert((threshold <= size), "Invalid threshold, it surprasses the hash size");
	unsigned __int64 returnValue = 1ULL;

	for (size_t i = 0; i < size; i++)
	{
		if (a[i] - b[i] != 0) {
			returnValue <<= 1;
		}
	}

	return (1ULL << threshold) >= returnValue;
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