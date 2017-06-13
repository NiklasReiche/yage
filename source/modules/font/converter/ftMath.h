#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include <graphics\Graphics.h>
#include <image\image.h>

#include "font.h"

namespace font
{
	struct Character;
	struct Font;

	struct Pixel
	{
		int x, y;
		unsigned char value;
	};

	struct Grid
	{
		int width = 0;
		int height = 0;
		std::vector<Pixel> data;

		Grid() {}
		Grid(int width, int height);
		Grid(img::Image image);

		void setPixel(Pixel pixel);
		Pixel getPixel(int x, int y);

		void toImage(img::Image & image);
	};

	bool operator==(const Pixel & left, const Pixel & right);
	bool operator!=(const Pixel & left, const Pixel & right);

	float dist(Pixel left, Pixel right);
	unsigned char getInverse(Pixel pixel);


	template <typename T>
	T clamp(T value, T lo, T hi);
	template <typename T, typename T2>
	T2 remap(T o_value, T o_lo, T o_hi, T2 n_lo, T2 n_hi);


	void distanceFunction(Grid & grid_in, Grid & grid_out, int x_min, int x_max, int y_min, int y_max, int searchRange = 16, float clampRange = 20.0f);

	void genDistanceFieldPerGlyph(img::Image & inputImage, std::map<unsigned char, TexMetrics> & glyphs, img::Image & outputImage, int searchRange = 16, float clampRange = 20.0f);
}