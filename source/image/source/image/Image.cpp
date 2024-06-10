#include "Image.h"

namespace yage::img
{
	Image::Image(int width, int height, int channels)
		: width(width), height(height), depth(channels)
	{
		imageData.resize(width * height * depth);
	}

	Image::Image(int width, int height, int channels, unsigned char* data)
		: width(width), height(height), depth(channels)
	{
		imageData.assign(data, data + (width * height * channels));
	}

	Image::Image(int width, int height, int channels, std::vector<unsigned char>& data)
		: width(width), height(height), depth(channels)
	{
		imageData = data;
	}

	void Image::flipHorizontally()
	{
		for (int h = 0; h < (height / 2); ++h) {
			for (int w = 0; w < width; ++w) {
				for (int d = 0; d < depth; ++d) {
					int h2 = height - 1 - h;
					unsigned char temp = this->operator()(h, w, d);
					this->operator()(h, w, d) = this->operator()(h2, w, d);
					this->operator()(h2, w, d) = temp;
				}
			}
		}
	}

	void Image::flipVertically()
	{
		for (int h = 0; h < height; ++h) {
			for (int w = 0; w < (width / 2); ++w) {
				for (int d = 0; d < depth; ++d) {
					int w2 = width - 1 - w;
					unsigned char temp = this->operator()(h, w, d);
					this->operator()(h, w, d) = this->operator()(h, w2, d);
					this->operator()(h, w2, d) = temp;
				}
			}
		}
	}

	std::unique_ptr<gl::ITexture2D> Image::toTexture(const std::shared_ptr<gl::ITextureCreator>& creator) const
	{
		gl::ImageFormat format;
		switch (depth) {
			case 1: format = gl::ImageFormat::R;
				break;
			case 2: format = gl::ImageFormat::RG;
				break;
			case 3: format = gl::ImageFormat::RGB;
				break;
			case 4: format = gl::ImageFormat::RGBA;
				break;
			default: format = gl::ImageFormat::UNDEFINED;
		}
		return creator->createTexture2D(width, height, format, {data(), imageData.size()});
	}


	int Image::getWidth() const
	{
		return width;
	}

	int Image::getHeight() const
	{
		return height;
	}

	int Image::getDepth() const
	{
		return depth;
	}

	std::size_t Image::getSize() const
	{
		return height * width * depth;
	}

	bool Image::isEmpty() const
	{
		return width == 0 || height == 0 || depth == 0;
	}

	unsigned char& Image::operator()(int h, int w, int d)
	{
		return imageData.at(h * width * depth + w * depth + d);
	}

	const unsigned char& Image::operator()(int h, int w, int d) const
	{
		return imageData.at(h * width * depth + w * depth + d);
	}

	unsigned char* Image::data()
	{
		return imageData.data();
	}

	const unsigned char* Image::data() const
	{
		return imageData.data();
	}

	std::ostream& operator<<(std::ostream& os, const Image& image)
	{
		os << "h: " << image.height << " | w: " << image.width << " | d: " << image.depth << "\n";

		for (int h = 0; h < image.height; h++) {
			for (int w = 0; w < image.width; w++) {
				for (int d = 0; d < image.depth; ++d) {
					std::string pad;
					int value = (int)image(h, w, d);
					if (value < 10) {
						pad = "  ";
					} else if (value < 100) {
						pad = " ";
					}
					std::cout << pad << value << " ";
				}
			}
			std::cout << "\n";
		}
		std::cout << std::flush;

		return os;
	}
}