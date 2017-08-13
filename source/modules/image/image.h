#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace img
{
	class Image
	{
	private:
		int width = 0;
		int height = 0;
		int channels = 0;
		std::shared_ptr<std::vector<unsigned char>> data;

	public:		
		Image() {}
		Image(int width, int height, int channels, unsigned char* data);
		Image(int width, int height, int channels, std::vector<unsigned char>& data);
		Image(const Image& other);
		Image(Image&& other);

		Image& operator=(const Image& other);
		friend std::ostream& operator<<(std::ostream & os, const Image & image);

		void flip();

		int getWidth() { return width; }
		int getHeight() { return height; }
		int getChannels() { return channels; }
		std::shared_ptr<std::vector<unsigned char>> getData() { return data; }
		unsigned char* getRawData() { return &(*data)[0]; }
	};

	std::ostream& operator<<(std::ostream & os, const Image & image);
}