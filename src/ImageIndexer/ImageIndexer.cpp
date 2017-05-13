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
	assert(size =< 64);
	unsigned __int64 returnValue = 1ULL;

	for (size_t i = 0; i < size; i++)
	{
		if (a[i] - b[i] != 0) {
			returnValue << 1;
		}
	}

	return (1ULL << threshold) >= returnValue;
}