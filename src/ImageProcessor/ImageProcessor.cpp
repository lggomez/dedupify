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

void ImageProcessor::ReduceToHash(const std::string currentPath, const std::vector<boost::filesystem::path>* filePaths, std::map<std::string, char*>* imageHashes) {
	ssize_t quantizationSize = 9;
	InitializeMagick(currentPath.c_str());

	for (boost::filesystem::path p : *filePaths)
	{
		try {
			std::string imagePath = p.string();
			cout << "\t Processing image: " << imagePath << std::endl;

			Image image(imagePath);
			image.modifyImage();
			image.resize(Geometry(quantizationSize, quantizationSize, quantizationSize, quantizationSize));
			image.quantizeColorSpace(GRAYColorspace);
			image.quantizeColors(256);
			image.quantize();

			ssize_t w = image.columns();
			ssize_t h = image.rows();
			char *hash = new char[((quantizationSize - 1) * (quantizationSize - 1)) * 2];

			for (ssize_t y = 0; y < quantizationSize; y++) {
				for (ssize_t x = 0; x < quantizationSize; x++) {
					Color pixelColor = image.pixelColor(x, y);
					Color nextPixelColor = image.pixelColor(x + 1, y);

					if (pixelColor.quantumBlue() > nextPixelColor.quantumBlue()) {
						hash[x + y*x] = '0';
					}
					else {
						hash[x + y*x] = '1';
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