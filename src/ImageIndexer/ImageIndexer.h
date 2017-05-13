#pragma once
#include <iostream>
#include <vector>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <../DeDupify/ThrowAssert.hpp>

using namespace std;

class ImageIndexer
{
public:
	ImageIndexer();
	~ImageIndexer();
	std::vector<vector<pair<std::string, char*>>> ImageIndexer::CreateIndex(const std::map<std::string, char*> imageHashes);
};

