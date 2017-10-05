#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include <graphics/Graphics.h>

namespace img
{
	class Image
	{
	private:
		int width = 0;
		int height = 0;
		int channels = 0;
		std::shared_ptr<std::vector<unsigned char>> data = nullptr;

	public:		
		Image() {}
		Image(int width, int height, int channels, unsigned char* data);
		Image(int width, int height, int channels, std::vector<unsigned char>& data);
		Image(const Image& other);
		Image(Image&& other);
		Image& operator=(const Image& other);
		friend std::ostream& operator<<(std::ostream & os, const Image & image);

		void flip();
		void addAlpha();

		gl::Texture toTexture(gl::GraphicsContext* glContext);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		int getChannels() const { return channels; }
		std::shared_ptr<std::vector<unsigned char>> getData() const { return data; }
		unsigned char* getRawData() { return &(*data)[0]; }

		bool isEmpty() const { return width == 0 || height == 0 || channels == 0; }
	};

	std::ostream& operator<<(std::ostream & os, const Image & image);
}