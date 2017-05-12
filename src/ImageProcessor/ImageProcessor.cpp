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

std::string NormalizePathEncoding(std::string imagePath) {
	int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, imagePath.c_str(),
		imagePath.length(), nullptr, 0);
	std::wstring utf16_str(size, '\0');
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, imagePath.c_str(),
		imagePath.length(), &utf16_str[0], size);

	int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), nullptr, 0,
		nullptr, nullptr);

	std::string utf8_str(utf8_size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), &utf8_str[0], utf8_size,
		nullptr, nullptr);

	return utf8_str;
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
			image.resize(Geometry(quantizationSize, quantizationSize -1, quantizationSize, quantizationSize-1));
			image.quantizeColorSpace(GRAYColorspace);
			image.quantizeColors(256);
			image.quantize();

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