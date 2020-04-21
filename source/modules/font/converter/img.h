#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace img
{
	unsigned char rgetBit(unsigned char c, int n);
	unsigned char getBit(unsigned char c, int n);

	void unpackBitmap(unsigned char* data, std::vector<unsigned char> & image, int rows, int width, int pitch);

	void flip(std::vector<unsigned char> & image, int height, int width, int channels);
}
