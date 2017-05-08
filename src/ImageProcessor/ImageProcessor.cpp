#include "stdafx.h"
#include "ImageProcessor.h"

using namespace std;

ImageProcessor::ImageProcessor()
{
}

ImageProcessor::~ImageProcessor()
{
}

void ImageProcessor::ReduceToHash(const std::string currentPath, const std::vector<boost::filesystem::path>* filePaths, std::map<std::string, char*>* imageHashes) {
	size_t quantizationSize = 8;
	Magick::InitializeMagick(currentPath.c_str());

	for (boost::filesystem::path p : *filePaths)
	{
		Magick::Image image;

		try {
			const std::string imagePath = p.string();
			image.read(imagePath);
			image.modifyImage();
			image.resize(Magick::Geometry(quantizationSize, quantizationSize, quantizationSize, quantizationSize));
			size_t w = image.columns();
			size_t h = image.rows();

			Magick::Pixels view(image);

			float luminSum = 0;
			unsigned offset = 0;
			size_t columns = image.columns(); size_t rows = image.rows();
			Magick::Quantum *pixels = image.getPixels(columns, rows, columns, rows);
			
			//Calculate luminance median from resized image's pixel colors
			for (ssize_t row = 0; row < rows; ++row)
			{
				for (ssize_t column = 0; column < columns; ++column)
				{
					offset = image.channels() * (w * row + column);
					luminSum += ((pixels[offset + 0] << 1) + pixels[offset + 0] + (pixels[offset + 1] << 2) + (pixels[offset + 2]) >> 3);
				}
			
			}

			float luminanceMedian = luminSum / (columns * rows);

			//Quantize image and calculate hash
			image.quantizeColorSpace(Magick::GRAYColorspace);
			image.quantizeColors(256);
			image.quantize();
			Magick::Quantum *grayscalePixels = image.getPixels(quantizationSize, quantizationSize, columns, rows);

			offset = 0;
			char *hash = new char[columns * rows];

			for (ssize_t row = 0; row < rows; ++row)
			{
				for (ssize_t column = 0; column < columns; ++column)
				{
					offset = image.channels() * (w * row + column);
					float epsilon = 0.0001;
					float luminance = ((grayscalePixels[offset + 0] << 1) + grayscalePixels[offset + 0] + (grayscalePixels[offset + 1] << 2) + (grayscalePixels[offset + 2]) >> 3);

					if (abs(luminance - luminanceMedian) > epsilon) {
						hash[row + column] = '0';
					}
					else {
						hash[row + column] = '1';
					}
				}
			}

			imageHashes->insert(std::pair<std::string, char*>(p.string(), hash));
		}
		catch (Magick::Exception &error_)
		{
			cout << "Caught exception: " << error_.what() << endl;
		}
	}
}