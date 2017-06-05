#include "img.h"


namespace img
{
	unsigned char getBit(unsigned char c, int n)
	{
		return (c >> n) & 1;
	}
	unsigned char rgetBit(unsigned char c, int n)
	{
		return ((c << n) & 128) >> 7;
	}


	void unpackBitmap(unsigned char* data, std::vector<unsigned char> & image, int rows, int width, int pitch)
	{
		int bit_pointer = 0;

		for (int i = 0; i < rows; i++)
		{
			unsigned char* row = &data[i*pitch];

			while (bit_pointer < width)
			{
				unsigned char v;

				if (rgetBit(row[bit_pointer / 8], bit_pointer % 8) == 1) {
					v = 255;
				}
				else if (rgetBit(row[bit_pointer / 8], bit_pointer % 8) == 0) {
					v = 0;
				}
				image.push_back(v);

				bit_pointer++;
			}
			bit_pointer = 0;
		}
	}

	void flip(std::vector<unsigned char> & image, int height, int width, int channels)
	{
		for (int i = 0; i < (height / 2); ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				// Swap values
				for (int k = 0; k < channels; ++k)
				{
					int top_index = (i * width + j) * channels + k;
					int bottom_index = ((height - 1 - i) * width + j) * channels + k;
					unsigned char top_value = image.at(top_index);
					unsigned char bottom_value = image.at(bottom_index);
					image.at(top_index) = bottom_value;
					image.at(bottom_index) = top_value;
				}
			}
		}
	}


	int readBMP(std::string filename, Image & image, FORCE_CHANNELS channel)
	{
		int err;
		bmp::Bitmap bitmap;
		err = bitmap.load(filename, channel);
		bitmap.toImage(image);
		return err;
	}
	int writeBMP(std::string filename, Image & image, FORCE_CHANNELS channel)
	{
		int err;
		bmp::Bitmap bitmap;
		bitmap.fromImage(image);
		err = bitmap.save(filename, channel);
		return err;
	}
}