#include "bmp.h"
#include <core/platform/FileException.h>
#include <cmath>

namespace yage::img::bmp
{
	/*****************************************
	**			8/24-bit Conversion			**
	*****************************************/

	int Bitmap::toBit8()
	{
		if (!isLoaded || infoheader.biBitCount != 24) {
			return 1;
		}

		int rows = infoheader.biHeight;
		int width = infoheader.biWidth;

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				float r = pixels.at(y * width + x + 0);
				float g = pixels.at(y * width + x + 1);
				float b = pixels.at(y * width + x + 2);
				unsigned char s = (unsigned char)std::round(0.21f * r + 0.72f * g + 0.07f * b);
				pixels.erase(pixels.begin() + (y * width + x + 1), pixels.begin() + (y * width + x + 2) + 1);
				pixels.at(y * width + x + 0) = s;
			}
		}

		createPalette(PALETTE_ENTRIES);

		infoheader.biBitCount = 8;
		infoheader.biSizeImage = rows * width;
		infoheader.biClrUsed = 256;
		infoheader.biClrImportant = 256;

		fileheader.bfSize -= rows * width * 2;
		fileheader.bfSize += PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT);

		fileheader.bfOffBits += PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT);

		return 0;
	}

	int Bitmap::toBit24()
	{
		if (!isLoaded || infoheader.biBitCount != 8) {
			return 1;
		}

		int rows = infoheader.biHeight;
		int width = infoheader.biWidth;

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				int index = (y * width + x) * 3;
				unsigned char s = pixels.at(index);
				pixels.insert(pixels.begin() + index + 1, s);
				pixels.insert(pixels.begin() + index + 2, s);
			}
		}

		palette.resize(0);

		infoheader.biBitCount = 24;
		infoheader.biSizeImage = rows * width * 3;
		infoheader.biClrUsed = 0;
		infoheader.biClrImportant = 0;

		fileheader.bfSize += rows * width * 2;
		fileheader.bfSize -= PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT);

		fileheader.bfOffBits -= PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT);

		return 0;
	}


	/*****************************************
	**					Helper				**
	*****************************************/

	void Bitmap::readVersion(platform::IBinaryFile& file)
	{
		uint32_t biSize;
		file.seek(14, platform::IFile::SeekOffset::BEG);
		file.read((char*)&biSize, sizeof(biSize));

		switch (biSize)
		{
		case 40:
			version = BMP_VERSION::BMP_V3;
			break;
		case 108:
			version = BMP_VERSION::BMP_V4;
			break;
		}
	}

	void Bitmap::createPalette(int size)
	{
		palette.resize(0);
		for (int i = 0; i < size; ++i)
		{
			PALETTE_ELEMENT element{ (uint8_t)i, (uint8_t)i, (uint8_t)i, 0x00 };
			palette.push_back(element);
		}
	}


	/*****************************************
	**				Read Helper				**
	*****************************************/

	void Bitmap::readMagicNumber(platform::IBinaryFile& file)
	{
		file.seek(0, platform::IFile::SeekOffset::BEG);
		file.read((char*)&magicNumber, sizeof(magicNumber));
	}
	void Bitmap::readBmpFileHeader(platform::IBinaryFile& file)
	{
		file.seek(2, platform::IFile::SeekOffset::BEG);
		file.read((char*)&fileheader.bfSize, sizeof(fileheader.bfSize));
		file.read((char*)&fileheader.bfReserved1, sizeof(fileheader.bfReserved1));
		file.read((char*)&fileheader.bfReserved2, sizeof(fileheader.bfReserved2));
		file.read((char*)&fileheader.bfOffBits, sizeof(fileheader.bfOffBits));
	}
	void Bitmap::readBmpInfoHeader(platform::IBinaryFile& file)
	{
		file.seek(14, platform::IFile::SeekOffset::BEG);
		file.read((char*)&infoheader.biSize, sizeof(infoheader.biSize));
		file.read((char*)&infoheader.biWidth, sizeof(infoheader.biWidth));
		file.read((char*)&infoheader.biHeight, sizeof(infoheader.biHeight));
		file.read((char*)&infoheader.biPlanes, sizeof(infoheader.biPlanes));
		file.read((char*)&infoheader.biBitCount, sizeof(infoheader.biBitCount));
		file.read((char*)&infoheader.biCompression, sizeof(infoheader.biCompression));
		file.read((char*)&infoheader.biSizeImage, sizeof(infoheader.biSizeImage));
		file.read((char*)&infoheader.biXPelsPerMeter, sizeof(infoheader.biXPelsPerMeter));
		file.read((char*)&infoheader.biYPelsPerMeter, sizeof(infoheader.biYPelsPerMeter));
		file.read((char*)&infoheader.biClrUsed, sizeof(infoheader.biClrUsed));
		file.read((char*)&infoheader.biClrImportant, sizeof(infoheader.biClrImportant));
	}
	void Bitmap::readBmpV4Header(platform::IBinaryFile & file)
	{
		file.seek(54, platform::IFile::SeekOffset::BEG);
		file.read((char*)&v4header, sizeof(v4header));
	}
	void Bitmap::readBitmask(platform::IBinaryFile & file)
	{
		file.seek(54, platform::IFile::SeekOffset::BEG);
		file.read((char*)&bitmask, sizeof(bitmask));
	}
	void Bitmap::readPalette(platform::IBinaryFile & file, int offset, int size)
	{
		file.seek(offset, platform::IFile::SeekOffset::BEG);
		palette.resize(0);
		for (int i = 0; i < size; ++i)
		{
			PALETTE_ELEMENT element{};
			file.read((char*)&element, sizeof(element));
			palette.push_back(element);
		}
	}

	void Bitmap::readImageData(platform::IBinaryFile & file)
	{
		int rows = infoheader.biHeight;
		int width = infoheader.biWidth;
		int bytes = infoheader.biBitCount / 8;
		int pad = (4 - (width * bytes) % 4) % 4;

		int pointer = fileheader.bfOffBits;
		file.seek(pointer, platform::IFile::SeekOffset::BEG);

		pixels.resize(0);
		pixels.reserve(rows * width * bytes);
		std::vector<uint8_t> pixel(bytes);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{

				for (int b = 0; b < bytes; ++b)
				{
					file.read((char*)&pixel[b], 1);
					++pointer;
				}
				for (int b = bytes - 1; b > -1; --b)
				{
					pixels.push_back(pixel[b]);
				}
			}
			pointer += pad;
			file.seek(pointer, platform::IFile::SeekOffset::BEG);
		}
	}
	void Bitmap::readImageDataIndexed(platform::IBinaryFile & file)
	{
		int rows = infoheader.biHeight;
		int width = infoheader.biWidth;
		int bytes = infoheader.biBitCount / 8;
		int pad = (4 - (width * bytes) % 4) % 4;

		int pointer = fileheader.bfOffBits;
		file.seek(pointer, platform::IFile::SeekOffset::BEG);

		pixels.resize(0);
		pixels.reserve(rows * width * bytes);
		std::vector<uint8_t> pixel(bytes);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				for (int b = 0; b < bytes; ++b)
				{
					file.read((char*)&pixel[b], 1);
					pixels.push_back(palette[pixel[b]].red);
					pixels.push_back(palette[pixel[b]].green);
					pixels.push_back(palette[pixel[b]].blue);
					++pointer;
				}
			}
			pointer += pad;
			file.seek(pointer, platform::IFile::SeekOffset::BEG);
		}

		infoheader.biBitCount = 24;
	}


	/*****************************************
	**				Write Helper			**
	*****************************************/

	void Bitmap::writeImageData(platform::IBinaryFile & file)
	{
		int rows = infoheader.biHeight;
		int width = infoheader.biWidth;
		int bytes = infoheader.biBitCount / 8;
		int pad = (4 - (width * bytes) % 4) % 4;

		int byteOrderOffset;
		if (bytes == 3) {
			byteOrderOffset = 2;
		}
		else if (bytes == 1) {
			byteOrderOffset = 0;
		}

		uint8_t emptyByte(0);

		file.seek(fileheader.bfOffBits, platform::IFile::SeekOffset::BEG);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				for (int b = 0; b < bytes; ++b)
				{
					int index = ((y * width + x) * bytes) + (byteOrderOffset - b);
					uint8_t spixel = pixels.at(index);
					file.write((char*)&spixel, sizeof(spixel));
				}
			}
			for (int p = 0; p < pad; ++p)
			{
				file.write((char*)&emptyByte, sizeof(emptyByte));
			}
		}
	}


	/*****************************************
	**				Load / Save				**
	*****************************************/

	void Bitmap::load(platform::IBinaryFile& file, FORCE_CHANNELS channel)
	{
		/* read magic number */
		readMagicNumber(file);
		if (magicNumber != 0x4D42) {
			throw platform::FileReadException("File is not a bmp image", "");
		} else {
			loadFlags.hasMagicNumber = true;
		}

		/* read file header */
		readBmpFileHeader(file);
		loadFlags.hasFileheader = true;

		/* read info header */
		readBmpInfoHeader(file);
		if (infoheader.biSize != 40 && infoheader.biSize != 108) {
			throw platform::FileReadException("bmp image is not V3 or V4", "");
		} else {
			loadFlags.hasInfoheader = true;
		}

		/* read v4 header if present */
		if (infoheader.biSize == 108) {
			readBmpV4Header(file);
			loadFlags.hasV4header = true;
		}

		/* read bitmask if present */
		if (infoheader.biCompression == 3) {
			readBitmask(file);
			loadFlags.hasBitmask = true;
		}

		/* read palette if present */
		if (infoheader.biBitCount == 8) {
			int offset = SIZE_FILEHEADER + infoheader.biSize;
			int size = infoheader.biClrUsed;
			if (loadFlags.hasBitmask) {
				offset += SIZE_BITMASK;
			}
			readPalette(file, offset, size);
			loadFlags.hasPalette = true;
		} else if (infoheader.biBitCount != 24) {
			throw platform::FileReadException("bmp image is not not 8 or 24 bit", "");
		}

		/* read image data */
		if (loadFlags.hasPalette) {
			readImageDataIndexed(file);
		} else {
			readImageData(file);
		}
		loadFlags.hasPixels = true;
		isLoaded = true;

		/* convert if force channels present */
		if (infoheader.biBitCount == 8 && channel == FORCE_CHANNELS::RGB) {
			toBit24();
		} else if (infoheader.biBitCount == 24 && channel == FORCE_CHANNELS::G) {
			toBit8();
		}
	}

	void Bitmap::save(platform::IBinaryFile& file, FORCE_CHANNELS channel)
	{
		if (!isLoaded){
			throw 1;
		}

		if (infoheader.biBitCount == 8 && channel == FORCE_CHANNELS::RGB) {
			toBit24();
		} else if (infoheader.biBitCount == 24 && channel == FORCE_CHANNELS::G) {
			toBit8();
		}

		file.seek(0, platform::IFile::SeekOffset::BEG);
		file.write((char*)&magicNumber, sizeof(magicNumber));
		file.write((char*)&fileheader, sizeof(fileheader));
		file.write((char*)&infoheader, sizeof(infoheader));

		if (infoheader.biBitCount == 8) {
			file.write((char*)&palette[0], PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT));
			writeImageData(file);
		} else if (infoheader.biBitCount == 24) {
			writeImageData(file);
		} else {
			std::cout << "BMP is not 8 or 24 bit" << std::endl;
			throw 1;
		}
	}

	Image img::bmp::Bitmap::toImage()
	{
		auto image = Image(infoheader.biWidth, std::abs(infoheader.biHeight), infoheader.biBitCount / 8, pixels);
		if (infoheader.biHeight > 0) {
			image.flipHorizontally();
		}
		return image;
	}

	void Bitmap::fromImage(Image image, bool)
	{
		int pad = (image.getWidth() * image.getDepth()) % (image.getDepth());
		int sizeImage = image.getHeight() * (image.getWidth() * image.getDepth() + pad);
		int colors = 0;
		int bitOffset = SIZE_FILEHEADER + SIZE_INFOHEADER + SIZE_V4HEADER;
		int fileSize = bitOffset + sizeImage;

		if (image.getDepth() == 1) {
			colors = 256;
			bitOffset += PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT);
			fileSize += PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT);
		}

		magicNumber = 0x4D42;

		fileheader.bfSize = fileSize;
		fileheader.bfReserved1 = 0;
		fileheader.bfReserved2 = 0;
		fileheader.bfOffBits = bitOffset;

		infoheader.biSize = 40;
		infoheader.biWidth = image.getWidth();
		infoheader.biHeight = image.getHeight();
		infoheader.biPlanes = 1;
		infoheader.biBitCount = image.getDepth() * 8;
		infoheader.biCompression = 0;
		infoheader.biSizeImage = sizeImage;
		infoheader.biXPelsPerMeter = 2835;
		infoheader.biYPelsPerMeter = 2835;
		infoheader.biClrUsed = colors;
		infoheader.biClrImportant = colors;

		v4header.RedMask = 0;
		v4header.GreenMask = 0;
		v4header.BlueMask = 0;
		v4header.AlphaMask = 0;
		v4header.CSType = 1;
		v4header.RedX = 0;
		v4header.RedY = 0;
		v4header.RedZ = 0;
		v4header.GreenX = 0;
		v4header.GreenY = 0;
		v4header.GreenZ = 0;
		v4header.BlueX = 0;
		v4header.BlueY = 0;
		v4header.BlueZ = 0;
		v4header.GammaRed = 0;
		v4header.GammaGreen = 0;
		v4header.GammaBlue = 0;

		if (image.getDepth() == 1) {
			createPalette(PALETTE_ENTRIES);
			loadFlags.hasPalette = true;
		}

		image.flipHorizontally();
		pixels.assign(image.data(), image.data() + (image.getWidth() * image.getDepth() * image.getHeight()));

		loadFlags.hasMagicNumber = true;
		loadFlags.hasFileheader = true;
		loadFlags.hasInfoheader = true;
		loadFlags.hasV4header = true;
		loadFlags.hasPixels = true;
		isLoaded = true;
	}
}
