#include "image.h"

namespace img
{
	Image::Image(int width, int height, int channels, unsigned char* data)
		: width(width), height(height), channels(channels)
	{
		this->data->assign(data, data + (width * height * channels));
	}
	Image::Image(int width, int height, int channels, std::vector<unsigned char>& data)
		: width(width), height(height), channels(channels)
	{
		this->data = std::make_shared<std::vector<unsigned char>>(data);
	}

	Image::Image(const Image& other)
	{
		this->width = other.width;
		this->height = other.height;
		this->channels = other.channels;
		this->data = std::make_shared<std::vector<unsigned char>>(*other.data);
	}

	Image::Image(Image&& other)
	{
		this->width = other.width;
		other.width = 0;
		this->height = other.height;
		other.height = 0;
		this->channels = other.channels;
		other.channels = 0;
		this->data = std::move(other.data);
	}

	Image& Image::operator=(const Image& other)
	{
		this->width = other.width;
		this->height = other.height;
		this->channels = other.channels;
		this->data = std::make_shared<std::vector<unsigned char>>(*other.data);
		return *this;
	}

	void Image::flip()
	{
		for (int i = 0; i < (height / 2); ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				for (int k = 0; k < channels; ++k)
				{
					int top_index = (i * width + j) * channels + k;
					int bottom_index = ((height - 1 - i) * width + j) * channels + k;
					// swap values
					unsigned char top_value = data->at(top_index);
					data->at(top_index) = data->at(bottom_index);
					data->at(bottom_index) = top_value;
				}
			}
		}
	}

	void Image::addAlpha()
	{
		if (channels == 1 || channels == 3) {
			for (int i = 0; i < height; ++i) {
				for (int j = 0; j < width; ++j) {
					data->insert(data->begin() + ((i * width + j) * channels + channels + (i * width + j)), 255); /* add (i * width + j) to index to adjust for already inserted values*/
				}
			}
			this->channels += 1;
		}
	}

	gl::Texture Image::toTexture(gl::GraphicsContext* glContext)
	{
		gl::ImageFormat format;
		switch (channels)
		{
		case 1:
			format = gl::ImageFormat::R;
			break;
		case 2:
			format = gl::ImageFormat::RG;
			break;
		case 3:
			format = gl::ImageFormat::RGB;
			break;
		case 4:
			format = gl::ImageFormat::RGBA;
			break;
		}
		return glContext->create2DTexture(getRawData(), width, height, format);
	}

	std::ostream& operator<<(std::ostream & os, const Image & image)
	{
		os << "w: " << image.width << "h: " << image.height << "b: " << image.channels << "\n";

		for (int i = 0; i < image.height; i++)
		{
			for (int j = 0; j < image.width; j++)
			{
				for (int k = 0; k < image.channels; ++k)
				{
					std::string pad = "";
					int value = (int)image.data->at(i * image.width * image.channels + j * image.channels + k);

					if (value < 10) {
						pad = "  ";
					}
					else if (value < 100) {
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