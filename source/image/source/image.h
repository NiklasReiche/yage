#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include <core/gl/Texture2D.h>
#include <core/gl/TextureCreator.h>

namespace img
{
	class Image
	{
	public:		
		Image() = default;
		Image(int width, int height, int channels, unsigned char* data);
		Image(int width, int height, int channels, std::vector<unsigned char>& data);
		Image(const Image& other);
		Image(Image&& other) noexcept;
		Image& operator=(const Image& other);
		friend std::ostream& operator<<(std::ostream & os, const Image & image);

		void flip();
		void addAlpha();

		std::unique_ptr<gl::ITexture2D> toTexture(const std::shared_ptr<gl::ITextureCreator>& creator) const;

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		int getChannels() const { return channels; }

		std::shared_ptr<std::vector<unsigned char>> getData() const { return data; }
		unsigned char* getRawData() { return &(*data)[0]; }

		[[nodiscard]]
		bool isEmpty() const { return width == 0 || height == 0 || channels == 0; }

	private:
		int width = 0;
		int height = 0;
		int channels = 0;
		std::shared_ptr<std::vector<unsigned char>> data = nullptr;
	};

	std::ostream& operator<<(std::ostream & os, const Image & image);
}