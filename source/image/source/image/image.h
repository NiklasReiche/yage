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
		Image(int width, int height, int channels);
		Image(int width, int height, int channels, unsigned char* data);
		Image(int width, int height, int channels, std::vector<unsigned char>& data);

		void flipHorizontally();

		void flipVertically();

		[[nodiscard]]
		std::unique_ptr<gl::ITexture2D> toTexture(const std::shared_ptr<gl::ITextureCreator>& creator) const;

		[[nodiscard]]
		int getWidth() const;

		[[nodiscard]]
		int getHeight() const;

		[[nodiscard]]
		int getDepth() const;

		[[nodiscard]]
		std::size_t getSize() const;

		[[nodiscard]]
		bool isEmpty() const;

		[[nodiscard]]
		unsigned char& operator()(int h, int w, int d);

		[[nodiscard]]
		const unsigned char& operator()(int h, int w, int d) const;

		[[nodiscard]]
		unsigned char* data();

		[[nodiscard]]
		const unsigned char* data() const;

		friend std::ostream& operator<<(std::ostream& os, const Image& image);

	private:
		int width = 0;
		int height = 0;
		int depth = 0;
		std::vector<unsigned char> imageData;
	};

	std::ostream& operator<<(std::ostream & os, const Image & image);
}