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
	throw_assert((threshold >= size), "Invalid threshold, it surprasses the hash size");
	unsigned __int64 returnValue = 1ULL;

	for (size_t i = 0; i < size; i++)
	{
		if (a[i] - b[i] != 0) {
			returnValue <<= 1;
		}
	}

	return (1ULL << threshold) >= returnValue;
}

std::vector<vector<pair<std::string, char*>>> ImageIndexer::CreateIndex(const std::map<std::string, char*> imageHashes) {
	const size_t HASH_SIZE = 64;
	const size_t HASH_THRESHOLD = 5;
	std::vector<vector<pair<std::string, char*>>> imageIndex((int)(imageHashes.size() / 2));

	for (auto const& imageHash : imageHashes)
	{
		pair<std::string, char*> imageIndexKey = make_pair(imageHash.first, imageHash.second);

		if (imageIndex.size() == 0) { //Initialize index
			std::vector<pair<std::string, char*>> imageIndexKeyList(1);
			imageIndexKeyList.push_back(imageIndexKey);
			imageIndex.push_back(imageIndexKeyList);
			continue;
		}

		for (auto const& imageIndexElement : imageIndex)
		{ //vector<pair<std::string, char*>>
			for (auto const& indexKey : imageIndexElement)
			{ //pair<std::string, char*>
				if (HashesAreSimilar(indexKey.second, imageIndexKey.second, HASH_SIZE, HASH_THRESHOLD)) {
					((vector<pair<std::string, char*>>)imageIndexElement).push_back(make_pair(indexKey.first, indexKey.second));
					break;
				}
			}

			std::vector<pair<std::string, char*>> imageIndexKeyList2(1);
			imageIndexKeyList2.push_back(imageIndexKey);
			imageIndex.push_back(imageIndexKeyList2);
		}
	}
}