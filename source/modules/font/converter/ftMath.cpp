#include "ftMath.h"

namespace font
{
	/*****************************************
	**			Pixel Grid Functions		**
	*****************************************/

	Grid::Grid(int width, int height)
		: width(width), height(height)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Pixel pixel{ x, y, 0 };
				this->data.push_back(pixel);
			}
		}
	}
	Grid::Grid(img::Image image)
		: width(image.width), height(image.height)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Pixel pixel{ x, y, image.data[y*width + x] };
				this->data.push_back(pixel);
			}
		}
	}

	void Grid::setPixel(Pixel pixel)
	{
		this->data[pixel.y*width + pixel.x] = pixel;
	}
	Pixel Grid::getPixel(int x, int y)
	{
		return this->data[y*width + x];
	}

	void Grid::toImage(img::Image & image)
	{
		image.width = this->width;
		image.height = this->height;
		image.depth = 1;
		image.data.resize(0);
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				image.data.push_back(getPixel(x, y).value);
			}
		}
	}


	/*****************************************
	**			Pixel Functions				**
	*****************************************/

	bool operator==(const Pixel & left, const Pixel & right)
	{
		return (left.x == right.x && left.y == right.y && left.value == right.value);
	}
	bool operator!=(const Pixel & left, const Pixel & right)
	{
		return !(left.x == right.x && left.y == right.y && left.value == right.value);
	}

	float dist(Pixel left, Pixel right)
	{
		int xd = left.x - right.x;
		int yd = left.y - right.y;
		return (float)sqrt((xd * xd) + (yd * yd));
	}
	unsigned char getInverse(Pixel pixel) {
		return 255 - pixel.value;
	}
	

	/*****************************************
	**		Common Math Functions			**
	*****************************************/

	template <typename T>
	T clamp(T value, T lo, T hi)
	{
		return std::max(lo, std::min(value, hi));
	}

	template <typename T, typename T2>
	T2 remap(T o_value, T o_lo, T o_hi, T2 n_lo, T2 n_hi)
	{
		T o_range = o_hi - o_lo;
		T2 n_range = n_hi - n_lo;
		T2 n_value = (T2)(((o_value - o_lo) * n_range) / (double)o_range) + n_lo;
		return n_value;
	}


	/*****************************************
	**			Distance Field 				**
	*****************************************/

	void distanceFunction(Grid & grid_in, Grid & grid_out, int g_x_min, int g_x_max, int g_y_min, int g_y_max, int searchRange, float clampRange)
	{
		// Generate Distance Field
		float maxDist = (float)sqrt((g_x_max - g_x_min) * (g_x_max - g_x_min) + (g_y_max - g_y_min) * (g_y_max - g_y_min));

		for (int g_y = g_y_min; g_y < g_y_max; ++g_y)
		{
			for (int g_x = g_x_min; g_x < g_x_max; ++g_x)
			{
				Pixel source = grid_in.getPixel(g_x, g_y);
				float distance = maxDist;

				// search area
				int l_x_min = std::max(g_x - searchRange, g_x_min);
				int l_x_max = std::min(g_x + searchRange, g_x_max);
				int l_y_min = std::max(g_y - searchRange, g_y_min);
				int l_y_max = std::min(g_y + searchRange, g_y_max);

				// traverse search area
				for (int l_y = l_y_min; l_y < l_y_max; l_y++)
				{
					for (int l_x = l_x_min; l_x < l_x_max; l_x++)
					{
						Pixel comp = grid_in.getPixel(l_x, l_y);

						if (comp.value == getInverse(source) && source != comp) {
							float newDistance = dist(source, comp);

							if (newDistance < sqrt(2) && source.value == 255) {
								newDistance = 0;
							}
							if (newDistance < distance) {
								distance = newDistance;
							}
						}
					}
				}

				// Negate distance if pixel is "inside"
				if (distance != maxDist && source.value == 255) {
					distance *= -1.0f;
				}

				// Clamp overflowing values
				distance = clamp(distance, -clampRange, clampRange);
				// Map values to 0-255
				int value = remap(distance, clampRange, -clampRange, 0, 255);

				// Polpulate output grid with new value
				Pixel pixel{ g_x, g_y, (unsigned char)value };
				grid_out.setPixel(pixel);
			}
		}
	}


	void genDistanceFieldPerGlyph(img::Image & inputImage, std::map<unsigned char, TexMetrics> & glyphs, img::Image & outputImage, int searchRange, float clampRange)
	{
		int total_width = inputImage.width;
		int total_height = inputImage.height;

		// Create input & output grid
		Grid grid_in(inputImage);
		Grid grid_out(total_width, total_height);

		// Traverse Glyphs
		int glyphCount = 1;
		for (auto it = glyphs.begin(); it != glyphs.end(); ++it)
		{
			unsigned char c = it->first;
			TexMetrics& coords = it->second;
			std::cout << "Processing Glyph " << "'" << c << "' - " << glyphCount << " of " << glyphs.size() << " ..." << std::endl;

			// Glyph boundary
			int g_x_min = (int)coords.left;
			int g_x_max = (int)coords.right;
			int g_y_min = 0;
			//int g_y_max = coords.bottom; // no idea why this isn't working
			int g_y_max = total_height;

			distanceFunction(grid_in, grid_out, g_x_min, g_x_max, g_y_min, g_y_max, searchRange, clampRange);

			glyphCount++;
		}

		// Convert grid_out to bitmap
		grid_out.toImage(outputImage);
	}
}