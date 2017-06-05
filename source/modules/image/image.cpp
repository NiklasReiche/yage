#include "image.h"


namespace img
{
	void Image::move(Image & image)
	{
		image.height = height;
		image.width = width;
		image.channels = channels;
		image.data = std::move(data);
	}

	Image Image::copy()
	{
		return Image{ width, height, channels, data };
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
					unsigned char top_value = data.at(top_index);
					data.at(top_index) = data.at(bottom_index);
					data.at(bottom_index) = top_value;
				}
			}
		}
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
					int value = (int)image.data.at(i * image.width * image.channels + j * image.channels + k);

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