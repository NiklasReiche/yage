#pragma once

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <cstdint>

#include <core/platform/IBinaryFile.h>

#include <image/Image.h>
#include <image/enum.h>

namespace yage::img::bmp
{
	enum class BMP_VERSION
	{
		BMP_V3,
		BMP_V3_NT,
		BMP_V4,
		BMP_V5
	};

	struct BITMAP_FILEHEADER
	{
		uint32_t bfSize;           /* Size of file */
		uint16_t bfReserved1;      /* Reserved */
		uint16_t bfReserved2;      /* Reserved */
		uint32_t bfOffBits;        /* Offset to bitmap data */
	};

	struct BITMAP_INFOHEADER
	{
		uint32_t biSize;           /* Size of info header */
		int32_t  biWidth;          /* Width of image */
		int32_t  biHeight;         /* Height of image */
		uint16_t biPlanes;         /* Number of color planes */
		uint16_t biBitCount;       /* Number of bits per pixel */
		uint32_t biCompression;    /* Type of compression to use */
		uint32_t biSizeImage;      /* Size of image data */
		int32_t  biXPelsPerMeter;  /* X pixels per meter */
		int32_t  biYPelsPerMeter;  /* Y pixels per meter */
		uint32_t biClrUsed;        /* Number of colors used */
		uint32_t biClrImportant;   /* Number of important colors */
	};

	struct BITMAP_V4HEADER
	{
		uint32_t RedMask;       /* Mask identifying bits of red component */
		uint32_t GreenMask;     /* Mask identifying bits of green component */
		uint32_t BlueMask;      /* Mask identifying bits of blue component */
		uint32_t AlphaMask;     /* Mask identifying bits of alpha component */
		uint32_t CSType;        /* Color space type */
		int32_t  RedX;          /* X coordinate of red endpoint */
		int32_t  RedY;          /* Y coordinate of red endpoint */
		int32_t  RedZ;          /* Z coordinate of red endpoint */
		int32_t  GreenX;        /* X coordinate of green endpoint */
		int32_t  GreenY;        /* Y coordinate of green endpoint */
		int32_t  GreenZ;        /* Z coordinate of green endpoint */
		int32_t  BlueX;         /* X coordinate of blue endpoint */
		int32_t  BlueY;         /* Y coordinate of blue endpoint */
		int32_t  BlueZ;         /* Z coordinate of blue endpoint */
		uint32_t GammaRed;      /* Gamma red coordinate scale value */
		uint32_t GammaGreen;    /* Gamma green coordinate scale value */
		uint32_t GammaBlue;     /* Gamma blue coordinate scale value */
	};

	struct BITMAP_BITMASK
	{
		uint32_t red;
		uint32_t green;
		uint32_t blue;
	};

	struct PALETTE_ELEMENT
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t reserved;
	};

	struct LOAD_FLAGS
	{
		bool hasMagicNumber = false;
		bool hasFileheader = false;
		bool hasInfoheader = false;
		bool hasV4header = false;
		bool hasBitmask = false;
		bool hasPalette = false;
		bool hasPixels = false;
	};

	class Bitmap
	{
	public:
		void load(platform::IBinaryFile& file, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);

		void save(platform::IBinaryFile& file, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);

		Image toImage();

		void fromImage(Image image, bool moveData = false);

	private:
		uint16_t magicNumber;
		BITMAP_FILEHEADER fileheader;
		BITMAP_INFOHEADER infoheader;
		BITMAP_V4HEADER v4header;
		BITMAP_BITMASK bitmask;
		std::vector<PALETTE_ELEMENT> palette;
		std::vector<unsigned char> pixels;

		const int SIZE_FILEHEADER = 14;
		const int SIZE_INFOHEADER = 40;
		const int SIZE_V4HEADER = 17 * sizeof(uint32_t);
		const int SIZE_BITMASK = 3 * sizeof(uint32_t);
		const int PALETTE_ENTRIES = 256;

		BMP_VERSION version;
		LOAD_FLAGS loadFlags;
		bool isLoaded = false;
		

		int toBit8();
		int toBit24();

		void readVersion(platform::IBinaryFile& file);
		void createPalette(int size);

		void readMagicNumber(platform::IBinaryFile& file);
		void readBmpFileHeader(platform::IBinaryFile& file);
		void readBmpInfoHeader(platform::IBinaryFile& file);
		void readBmpV4Header(platform::IBinaryFile& file);
		void readBitmask(platform::IBinaryFile& file);
		void readPalette(platform::IBinaryFile& file, int offset, int size);
		void readImageData(platform::IBinaryFile& file);
		void readImageDataIndexed(platform::IBinaryFile& file);

		void writeImageData(platform::IBinaryFile& file);
	};
}
