#include "stdafx.h"
#include "ImageProcessor.h"

using namespace std;
using namespace Magick;
using namespace MagickCore;

ImageProcessor::ImageProcessor()
{
}

ImageProcessor::~ImageProcessor()
{
}

wstring ConvertToLPCWSTR(const string& s)
{
	int slength = static_cast<int>(s.length()) + 1;

	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring convertedString(buf);
	delete[] buf;

	return convertedString;
}

string NormalizePathEncoding(string imagePath) {
	long       length = 0;
	wchar_t*   buffer = nullptr;

	// First obtain the size needed by passing NULL and 0.
	length = GetShortPathName(ConvertToLPCWSTR(imagePath).c_str(), nullptr, 0);

	// Dynamically allocate the correct size 
	// (terminating null char was included in length)
	buffer = new wchar_t[length];

	// Now simply call again using same long path.
	length = GetShortPathName(ConvertToLPCWSTR(imagePath).c_str(), buffer, length);

	using Codecvt = codecvt_utf8<wchar_t>;
	wstring_convert<Codecvt, wchar_t> converter;

	string returnString = converter.to_bytes(buffer);

	if (buffer) {
		delete[] buffer;
	}

	return returnString;
}

#if _DEBUG
void ReduceToHashMock(const std::string& currentPath, const std::vector<boost::filesystem::path>& filePaths, std::map<std::string, char*>& imageHashes) {
	char *hash1 = new char[HASH_SIZE + 1];
	std::fill(hash1, hash1 + HASH_SIZE, '0');
	hash1[HASH_SIZE] = '\0';
	imageHashes.insert(std::pair<std::string, char*>(std::string("C:\1.png"), hash1));

	char *hash2 = new char[HASH_SIZE + 1];
	std::fill(hash2, hash2 + HASH_SIZE, '0');
	for (size_t i = 1; i <= 7; i++) { hash2[1 * i] = '1'; hash2[2 * i] = '1'; hash2[5 * i] = '1'; hash2[6 * i] = '1'; hash2[7 * i] = '1'; hash2[8 * i] = '1'; }
	hash2[HASH_SIZE] = '\0';
	imageHashes.insert(std::pair<std::string, char*>(std::string("C:\2.png"), hash2));

	char *hash3 = new char[HASH_SIZE + 1];
	std::fill(hash3, hash3 + HASH_SIZE, '0');
	hash3[HASH_SIZE] = '\0';
	imageHashes.insert(std::pair<std::string, char*>(std::string("C:\1 edit.png"), hash3));

	char *hash4 = new char[HASH_SIZE + 1];
	std::fill(hash4, hash4 + HASH_SIZE, '0');
	hash4[HASH_SIZE] = '\0';
	hash4[15] = '1'; hash4[25] = '1'; hash4[30] = '1';
	imageHashes.insert(std::pair<std::string, char*>(std::string("C:\1 edit2.png"), hash4));

	char *hash5 = new char[HASH_SIZE + 1];
	std::fill(hash5, hash5 + HASH_SIZE, '0');
	hash5[HASH_SIZE] = '\0';
	for (size_t i = 1; i <= 5; i++) { hash5[5 * i] = '1'; hash5[3 * i] = '1'; hash5[10 * i] = '1'; hash5[9 * i] = '1'; hash5[12 * i] = '1'; hash5[11 * i] = '1'; }
	imageHashes.insert(std::pair<std::string, char*>(std::string("C:\3.png"), hash5));

	char *hash6 = new char[HASH_SIZE + 1];
	std::fill(hash6, hash6 + HASH_SIZE, '0');
	hash6[HASH_SIZE] = '\0';
	for (size_t i = 1; i <= 5; i++) { hash6[5 * i] = '1'; hash6[3 * i] = '1'; hash6[10 * i] = '1'; hash6[9 * i] = '1'; hash6[12 * i] = '1'; hash6[11 * i] = '1'; }
	hash6[15] = '1'; hash6[25] = '1'; hash6[30] = '1';
	imageHashes.insert(std::pair<std::string, char*>(std::string("C:\3 edit.png"), hash6));
}
#endif

void ImageProcessor::ReduceToHash(const string& currentPath, const vector<boost::filesystem::path>& filePaths, map<string, char*>& imageHashes) {
#if _DEBUG
	cout << "WARNING: MOCK MODE - Extracted file paths will be ignored" << std::endl;
	ReduceToHashMock(currentPath, filePaths, imageHashes);
	return;
#endif

	InitializeMagick(currentPath.c_str());

	for (const boost::filesystem::path& p : filePaths)
	{
		string imagePath = p.string();
		cout << "\t Opening image: " << imagePath << endl;
		auto normalizedPath = NormalizePathEncoding(imagePath);
		Magick::Image image;

		try {
			Magick::Image subimage(imagePath);
			image = subimage;
		}
		catch (Exception &error_)
		{
			try {
				Magick::Image subimage(normalizedPath);
				image = subimage;
			}
			catch (Exception &suberror_)
			{
				cout << "Could not open image: " << suberror_.what() << endl;
				continue;
			}
		}

		try {
			///*DEBUG*/cout << "\t\t Image opened" << std::endl;
			image.modifyImage();
			image.quantizeColorSpace(GRAYColorspace);
			image.quantizeColors(256);
			image.quantize();
			Geometry newSize = Geometry(QUANTIZATION_SIZE, QUANTIZATION_SIZE, 0, 0);
			newSize.aspect(true);
			image.resize(newSize);
			
			ssize_t w = image.columns();
			ssize_t h = image.rows();

			char *hash = new char[HASH_SIZE + 1];
			fill(hash, hash + HASH_SIZE, '0');
			hash[HASH_SIZE] = '\0';
			
			for (ssize_t y = 0; y < QUANTIZATION_SIZE; ++y) {
				for (ssize_t x = 0; x < QUANTIZATION_SIZE - 1; ++x) {
					Color pixelColor = image.pixelColor(x, y);
					Color nextPixelColor = image.pixelColor(x + 1, y);

					ssize_t index = x*y + y;

					if (pixelColor.quantumBlue() < nextPixelColor.quantumBlue()) {
						hash[index] = '1';
					}
				}
			}

			int hashOffset = HASH_SIZE / 2;

			for (ssize_t x = 0; x < QUANTIZATION_SIZE; ++x) {
				for (ssize_t y = 0; y < QUANTIZATION_SIZE - 1; ++y) {
					Color pixelColor = image.pixelColor(x, y);
					Color nextPixelColor = image.pixelColor(x, y + 1);

					ssize_t index = hashOffset + x*y + y;

					if (pixelColor.quantumBlue() < nextPixelColor.quantumBlue()) {
						hash[index] = '1';
					}
				}
			}

			imageHashes.insert(pair<string, char*>(p.string(), hash));
		}
		catch (Exception &error_)
		{
			cout << "Caught exception: " << error_.what() << endl;
		}
	}
}

void ImageProcessor::ReduceWithDFT(const string& currentPath, const vector<boost::filesystem::path>& filePaths, map<string, pair<double_t, double_t*>>& imageMagnitudes) {
	MagickCoreGenesis(currentPath.c_str(), MagickTrue);
	InitializeMagick(currentPath.c_str());
	ExceptionInfo *exceptionInfo = AcquireExceptionInfo();

	for (const boost::filesystem::path& p : filePaths)
	{
		string imagePath = p.string();
		cout << "\t Opening image: " << imagePath << endl;
		auto normalizedPath = NormalizePathEncoding(imagePath);
		Magick::Image image;
		
		try {
			Magick::Image subimage(imagePath);
			image = subimage;
		}
		catch (Exception &error_)
		{
			try {
				Magick::Image subimage(normalizedPath);
				image = subimage;
			}
			catch (Exception &suberror_)
			{
				cout << "Could not open image: " << suberror_.what() << endl;
				continue;
			}
		}

		try {
			///*DEBUG*/cout << "\t\t Image opened" << std::endl;
			image.modifyImage();
			Geometry newSize = Geometry(DFT_IMAGE_WIDTH, DFT_IMAGE_WIDTH, 0, 0);
			newSize.aspect(true);
			image.resize(newSize);
			image = ForwardFourierTransformImage(image.constImage(), MagickTrue, exceptionInfo);

			///*DEBUG*/cout << "\t\t DFT applied: current size is " << w << "x" << h << std::endl;
			size_t imageSize = DFT_IMAGE_HEIGHT * DFT_IMAGE_WIDTH;

			///*DEBUG*/cout << "\t\t Initializing magnitudes" << std::endl;
			double_t *magnitudes = new double_t[imageSize + 1];
			magnitudes[imageSize] = '\0';
			double_t magnitudeMedian = 0.0;
			double_t totalMagnitude = 0.0;

			///*DEBUG*/cout << "\t\t Setting magnitudes" << std::endl;
			for (ssize_t x = 0; x < DFT_IMAGE_WIDTH; ++x) {
				for (ssize_t y = 0; y < DFT_IMAGE_HEIGHT; ++y) {
					Quantum* quantum = image.getPixels(x, y, 1, 1);
					magnitudes[x*y + y] = *quantum;
					//totalMagnitude += magnitudes[x*y + y];
					Color pixelColor = image.pixelColor(x, y);
					totalMagnitude += pixelColor.quantumRed() + pixelColor.quantumGreen() + pixelColor.quantumBlue();
				}
			}

			///*DEBUG*/cout << "\t\t Doing final assignments" << imagePath << std::endl;
			magnitudeMedian = totalMagnitude / static_cast<double_t>(imageSize);
			imageMagnitudes[imagePath] = pair<double_t, double_t*>(magnitudeMedian, magnitudes);

			///*DEBUG*/cout << "\t\t Magnitude median: " << magnitudeMedian << std::endl;
		}
		catch (Exception &error_)
		{
			cout << "Caught exception: " << error_.what() << endl;
		}
	}

	DestroyExceptionInfo(exceptionInfo);
	MagickCoreTerminus();
}