#pragma once
#include <iostream>
#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <../DeDupify/ThrowAssert.hpp>

using namespace std;

class ImageIndexer
{
private:
	const size_t HASH_SIZE = 128;
	const size_t HASH_THRESHOLD = 3;
public:
	ImageIndexer();
	~ImageIndexer();
	std::vector<vector<pair<std::string, char*>>> ImageIndexer::CreateIndex(std::map<std::string, char*> imageHashes);
};

