#include "stdafx.h"
#include "ImageProcessor.h"

using namespace std;
using namespace Magick;

ImageProcessor::ImageProcessor()
{
}

ImageProcessor::~ImageProcessor()
{
}

const size_t QUANTIZATION_SIZE = 9;
const size_t HASH_SIZE = (QUANTIZATION_SIZE - 1) * (QUANTIZATION_SIZE - 1);

std::wstring ConvertToLPCWSTR(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;

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

#if _DEBUG
void ReduceToHashMock(const std::string currentPath, const std::vector<boost::filesystem::path>* filePaths, std::map<std::string, char*>* imageHashes) {
	char *hash1 = new char[HASH_SIZE + 1];
	std::fill(hash1, hash1 + HASH_SIZE, '0');
	hash1[HASH_SIZE] = '\0';
	imageHashes->insert(std::pair<std::string, char*>(std::string("C:\1.png"), hash1));

	char *hash2 = new char[HASH_SIZE + 1];
	std::fill(hash2, hash2 + HASH_SIZE, '0');
	for (size_t i = 1; i <= 7; i++) { hash2[1 * i] = '1'; hash2[2 * i] = '1'; hash2[5 * i] = '1'; hash2[6 * i] = '1'; hash2[7 * i] = '1'; hash2[8 * i] = '1'; }
	hash2[HASH_SIZE] = '\0';
	imageHashes->insert(std::pair<std::string, char*>(std::string("C:\2.png"), hash2));

	char *hash3 = new char[HASH_SIZE + 1];
	std::fill(hash3, hash3 + HASH_SIZE, '0');
	hash3[HASH_SIZE] = '\0';
	imageHashes->insert(std::pair<std::string, char*>(std::string("C:\1 edit.png"), hash3));
}
#endif

void ImageProcessor::ReduceToHash(const std::string currentPath, const std::vector<boost::filesystem::path>* filePaths, std::map<std::string, char*>* imageHashes) {
#if _DEBUG
	cout << "WARNING: MOCK MODE - Extracted file paths will be ignored" << std::endl;
	ReduceToHashMock(currentPath, filePaths, imageHashes);
	return;
#endif

	InitializeMagick(currentPath.c_str());

	for (boost::filesystem::path p : *filePaths)
	{
		try {
			std::string imagePath = p.string();
#if _DEBUG
			cout << "\t Processing image: " << imagePath << std::endl;
#endif
			Magick::Image image(NormalizePathEncoding(imagePath));
			image.modifyImage();
			image.quantizeColorSpace(GRAYColorspace);
			image.quantizeColors(256);
			image.quantize();
			image.resize(Geometry(QUANTIZATION_SIZE, QUANTIZATION_SIZE -1, QUANTIZATION_SIZE, QUANTIZATION_SIZE-1));

			ssize_t w = image.columns();
			ssize_t h = image.rows();

			char *hash = new char[HASH_SIZE + 1];
			std::fill(hash, hash + HASH_SIZE, '0');
			hash[HASH_SIZE] = '\0';
			
			for (ssize_t y = 0; y < QUANTIZATION_SIZE; y++) {
				for (ssize_t x = 0; x < QUANTIZATION_SIZE-1; x++) {
					Color pixelColor = image.pixelColor(x, y);
					Color nextPixelColor = image.pixelColor(x + 1, y);

					ssize_t index = x*y + x;

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