#include "stdafx.h"
#include "ImageProcessor.h"
#include <Windows.h>
#include <codecvt>

using namespace std;
using namespace Magick;

ImageProcessor::ImageProcessor()
{
}

ImageProcessor::~ImageProcessor()
{
}

std::wstring ConvertToLPCWSTR(const std::string& s)
{
	ssize_t len;
	ssize_t slength = (int)s.length() + 1;

	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring convertedString(buf);
	delete[] buf;

	return convertedString;
}

std::string NormalizePathEncoding(std::string imagePath) {
	long     length = 0;
	wchar_t*   buffer = NULL;

	// First obtain the size needed by passing NULL and 0.
	length = GetShortPathName(ConvertToLPCWSTR(imagePath).c_str(), NULL, 0);

	// Dynamically allocate the correct size 
	// (terminating null char was included in length)
	buffer = new wchar_t[length];

	// Now simply call again using same long path.
	length = GetShortPathName(ConvertToLPCWSTR(imagePath).c_str(), buffer, length);

	using Codecvt = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<Codecvt, wchar_t> converter;

	std::string returnString = converter.to_bytes(buffer);

	if (buffer) {
		delete[] buffer;
	}

	return returnString;
}

void ImageProcessor::ReduceToHash(const std::string currentPath, const std::vector<boost::filesystem::path>* filePaths, std::map<std::string, char*>* imageHashes) {
	ssize_t quantizationSize = 9;
	ssize_t hashSize = (quantizationSize - 1) * (quantizationSize - 1);

	InitializeMagick(currentPath.c_str());

	for (boost::filesystem::path p : *filePaths)
	{
		try {
			std::string imagePath = p.string();
			cout << "\t Processing image: " << imagePath << std::endl;

			Magick::Image image(NormalizePathEncoding(imagePath));
			image.modifyImage();
			image.quantizeColorSpace(GRAYColorspace);
			image.quantizeColors(256);
			image.quantize();
			image.resize(Geometry(quantizationSize, quantizationSize -1, quantizationSize, quantizationSize-1));

			ssize_t w = image.columns();
			ssize_t h = image.rows();

			char *hash = new char[hashSize + 1];
			std::fill(hash, hash + hashSize, '0');
			hash[hashSize] = '\0';
			
			for (ssize_t y = 0; y < quantizationSize; y++) {
				for (ssize_t x = 0; x < quantizationSize-1; x++) {
					Color pixelColor = image.pixelColor(x, y);
					Color nextPixelColor = image.pixelColor(x + 1, y);

					ssize_t index = x*y + x;
					assert(index < hashSize);

					if (pixelColor.quantumBlue() < nextPixelColor.quantumBlue()) {
						hash[index] = '1';
					}
				}
			}

			imageHashes->insert(std::pair<std::string, char*>(p.string(), hash));
		}
		catch (Exception &error_)
		{
			cout << "Caught exception: " << error_.what() << endl;
		}
	}
}