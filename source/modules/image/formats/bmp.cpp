#include "bmp.h"
#include "platform/FileLoader.h"

namespace img::bmp
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

	void Bitmap::readVersion(sys::File & file)
	{
		uint32_t biSize;
		file->seek(14, sys::IFile::SeekOffset::BEG);
		file->read((char*)&biSize, sizeof(biSize));

		switch (biSize)
		{
		case 40:
			version = BMP_VERSION::BMP_V3;
		case 108:
			version = BMP_VERSION::BMP_V4;
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

	int Bitmap::checkError(sys::File & file)
	{
		// TODO
		//if (file.getError()) {
		//	return 1;
		//}
		return 0;
	}


	/*****************************************
	**				Read Helper				**
	*****************************************/

	void Bitmap::readMagicNumber(sys::File & file)
	{
		file->seek(0, sys::IFile::SeekOffset::BEG);
		file->read((char*)&magicNumber, sizeof(magicNumber));
	}
	void Bitmap::readBmpFileHeader(sys::File & file)
	{
		file->seek(2, sys::IFile::SeekOffset::BEG);
		file->read((char*)&fileheader.bfSize, sizeof(fileheader.bfSize));
		file->read((char*)&fileheader.bfReserved1, sizeof(fileheader.bfReserved1));
		file->read((char*)&fileheader.bfReserved2, sizeof(fileheader.bfReserved2));
		file->read((char*)&fileheader.bfOffBits, sizeof(fileheader.bfOffBits));
	}
	void Bitmap::readBmpInfoHeader(sys::File & file)
	{
		file->seek(14, sys::IFile::SeekOffset::BEG);
		file->read((char*)&infoheader.biSize, sizeof(infoheader.biSize));
		file->read((char*)&infoheader.biWidth, sizeof(infoheader.biWidth));
		file->read((char*)&infoheader.biHeight, sizeof(infoheader.biHeight));
		file->read((char*)&infoheader.biPlanes, sizeof(infoheader.biPlanes));
		file->read((char*)&infoheader.biBitCount, sizeof(infoheader.biBitCount));
		file->read((char*)&infoheader.biCompression, sizeof(infoheader.biCompression));
		file->read((char*)&infoheader.biSizeImage, sizeof(infoheader.biSizeImage));
		file->read((char*)&infoheader.biXPelsPerMeter, sizeof(infoheader.biXPelsPerMeter));
		file->read((char*)&infoheader.biYPelsPerMeter, sizeof(infoheader.biYPelsPerMeter));
		file->read((char*)&infoheader.biClrUsed, sizeof(infoheader.biClrUsed));
		file->read((char*)&infoheader.biClrImportant, sizeof(infoheader.biClrImportant));
	}
	void Bitmap::readBmpV4Header(sys::File & file)
	{
		file->seek(54, sys::IFile::SeekOffset::BEG);
		file->read((char*)&v4header, sizeof(v4header));
	}
	void Bitmap::readBitmask(sys::File & file)
	{
		file->seek(54, sys::IFile::SeekOffset::BEG);
		file->read((char*)&bitmask, sizeof(bitmask));
	}
	void Bitmap::readPalette(sys::File & file, int offset, int size)
	{
		file->seek(offset, sys::IFile::SeekOffset::BEG);
		palette.resize(0);
		for (int i = 0; i < size; ++i)
		{
			PALETTE_ELEMENT element;
			file->read((char*)&element, sizeof(element));
			palette.push_back(element);
		}
	}

	void Bitmap::readImageData(sys::File & file)
	{
		int rows = infoheader.biHeight;
		int width = infoheader.biWidth;
		int bytes = infoheader.biBitCount / 8;
		int pad = (4 - (width * bytes) % 4) % 4;

		int pointer = fileheader.bfOffBits;
		file->seek(pointer, sys::IFile::SeekOffset::BEG);

		pixels.resize(0);
		pixels.reserve(rows * width * bytes);
		std::vector<uint8_t> pixel(bytes);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{

				for (int b = 0; b < bytes; ++b)
				{
					file->read((char*)&pixel[b], 1);
					++pointer;
				}
				for (int b = bytes - 1; b > -1; --b)
				{
					pixels.push_back(pixel[b]);
				}
			}
			pointer += pad;
			file->seek(pointer, sys::IFile::SeekOffset::BEG);
		}
	}
	void Bitmap::readImageDataIndexed(sys::File & file)
	{
		int rows = infoheader.biHeight;
		int width = infoheader.biWidth;
		int bytes = infoheader.biBitCount / 8;
		int pad = (4 - (width * bytes) % 4) % 4;

		int pointer = fileheader.bfOffBits;
		file->seek(pointer, sys::IFile::SeekOffset::BEG);

		pixels.resize(0);
		pixels.reserve(rows * width * bytes);
		std::vector<uint8_t> pixel(bytes);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				for (int b = 0; b < bytes; ++b)
				{
					file->read((char*)&pixel[b], 1);
					pixels.push_back(palette[pixel[b]].red);
					pixels.push_back(palette[pixel[b]].green);
					pixels.push_back(palette[pixel[b]].blue);
					++pointer;
				}
			}
			pointer += pad;
			file->seek(pointer, sys::IFile::SeekOffset::BEG);
		}

		infoheader.biBitCount = 24;
	}


	/*****************************************
	**				Write Helper			**
	*****************************************/

	void Bitmap::writeImageData(sys::File & file)
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

		file->seek(fileheader.bfOffBits, sys::IFile::SeekOffset::BEG);

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				for (int b = 0; b < bytes; ++b)
				{
					int index = ((y * width + x) * bytes) + (byteOrderOffset - b);
					uint8_t spixel = pixels.at(index);
					file->write((char*)&spixel, sizeof(spixel));
				}
			}
			for (int p = 0; p < pad; ++p)
			{
				file->write((char*)&emptyByte, sizeof(emptyByte));
			}
		}
	}


	/*****************************************
	**				Load / Save				**
	*****************************************/

	int Bitmap::load(const std::string & filename, FORCE_CHANNELS channel)
	{
		sys::File file = sys::FileLoader::openFile(filename);

		if (!file->isOpen()) {
			std::cout << "Could not open '" << filename << "'" << std::endl;
			return 1;
		}

		/* read magic number */
		readMagicNumber(file);

		if (checkError(file)) {
			std::cout << "Could not read file identifier '" << filename << "'" << std::endl;
			return 2;
		}
		else if (magicNumber != 0x4D42) {
			std::cout << "File is not .BMP" << std::endl;
			return 1;
		}
		else {
			loadFlags.hasMagicNumber = true;
		}

		/* read file header */
		readBmpFileHeader(file);
		if (checkError(file)) {
			std::cout << "Could not read file header '" << filename << "'" << std::endl;
			return 2;
		}
		else {
			loadFlags.hasFileheader = true;
		}

		/* read info header */
		readBmpInfoHeader(file);
		if (checkError(file)) {
			std::cout << "Could not read info header '" << filename << "'" << std::endl;
			return 2;
		}
		else if (infoheader.biSize != 40 && infoheader.biSize != 108) {
			std::cout << "BMP is not V3 or V4" << std::endl;
			return 1;
		}
		else {
			loadFlags.hasInfoheader = true;
		}

		/* read v4 header if present */
		if (infoheader.biSize == 108) {
			readBmpV4Header(file);
			if (checkError(file)) {
				std::cout << "Could not read v4 header '" << filename << "'" << std::endl;
				return 2;
			}
			else {
				loadFlags.hasV4header = true;
			}
		}

		/* read bitmask if present */
		if (infoheader.biCompression == 3) {
			readBitmask(file);
			if (checkError(file)) {
				std::cout << "Could not read bitmask field '" << filename << "'" << std::endl;
				return 2;
			}
			else {
				loadFlags.hasBitmask = true;
			}
		}

		/* read palette if present */
		if (infoheader.biBitCount == 8) {
			int offset = SIZE_FILEHEADER + infoheader.biSize;
			int size = infoheader.biClrUsed;
			if (loadFlags.hasBitmask) {
				offset += SIZE_BITMASK;
			}

			readPalette(file, offset, size);
			if (checkError(file)) {
				std::cout << "Could not read bitmask field '" << filename << "'" << std::endl;
				return 2;
			}
			else {
				loadFlags.hasPalette = true;
			}
		}
		else if (infoheader.biBitCount != 24) {
			std::cout << "BMP is not 8 or 24 bit" << std::endl;
			return 1;
		}

		/* read image data */
		if (loadFlags.hasPalette) {
			readImageDataIndexed(file);
		}
		else {
			readImageData(file);
		}
		if (checkError(file)) {
			std::cout << "Could not read image data '" << filename << "'" << std::endl;
			return 2;
		}
		else {
			loadFlags.hasPixels = true;
		}
		isLoaded = true;

		/* convert if force channels present */
		if (infoheader.biBitCount == 8 && channel == FORCE_CHANNELS::RGB) {
			toBit24();
		}
		else if (infoheader.biBitCount == 24 && channel == FORCE_CHANNELS::G) {
			toBit8();
		}

		return 0;
	}

	int Bitmap::save(const std::string & filename, FORCE_CHANNELS channel)
	{
		sys::File file = sys::FileLoader::openFile(filename, sys::IFile::AccessMode::WRITE);

		if (file->isOpen() && isLoaded)
		{
			if (infoheader.biBitCount == 8 && channel == FORCE_CHANNELS::RGB) {
				toBit24();
			}
			else if (infoheader.biBitCount == 24 && channel == FORCE_CHANNELS::G) {
				toBit8();
			}

			file->seek(0, sys::IFile::SeekOffset::BEG);
			file->write((char*)&magicNumber, sizeof(magicNumber));
			file->write((char*)&fileheader, sizeof(fileheader));
			file->write((char*)&infoheader, sizeof(infoheader));

			if (infoheader.biBitCount == 8) {
				file->write((char*)&palette[0], PALETTE_ENTRIES * sizeof(PALETTE_ELEMENT));
				writeImageData(file);
			}
			else if (infoheader.biBitCount == 24) {
				writeImageData(file);
			}
			else {
				std::cout << "BMP is not 8 or 24 bit" << std::endl;
				return 1;
			}
			return 0;
		}
		else {
			std::cout << "Could not write file '" << filename << "'" << std::endl;
			return 1;
		}
	}

	void img::bmp::Bitmap::toImage(Image & image)
	{
		image = Image(infoheader.biWidth, std::abs(infoheader.biHeight), infoheader.biBitCount / 8, pixels);
		if (infoheader.biHeight > 0) {
			image.flip();
		}
	}
	void Bitmap::fromImage(Image & image, bool moveData)
	{
		int pad = (image.getWidth() * image.getChannels()) % (image.getChannels());
		int sizeImage = image.getHeight() * (image.getWidth() * image.getChannels() + pad);
		int colors = 0;
		int bitOffset = SIZE_FILEHEADER + SIZE_INFOHEADER + SIZE_V4HEADER;
		int fileSize = bitOffset + sizeImage;

		if (image.getChannels() == 1) {
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
		infoheader.biBitCount = image.getChannels() * 8;
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

		if (image.getChannels() == 1) {
			createPalette(PALETTE_ENTRIES);
			loadFlags.hasPalette = true;
		}

		image.flip();
		if (moveData) {
			pixels = *std::move(image.getData());
		}
		else {
			pixels = *image.getData();
		}

		loadFlags.hasMagicNumber = true;
		loadFlags.hasFileheader = true;
		loadFlags.hasInfoheader = true;
		loadFlags.hasV4header = true;
		loadFlags.hasPixels = true;
		isLoaded = true;
	}
}
