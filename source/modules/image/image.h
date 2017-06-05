#pragma once

#include <string>
#include <vector>
#include <iostream>


namespace img
{
	struct Image
	{
		int width;
		int height;
		int channels;
		std::vector<unsigned char> data;

		void flip();
		void move(Image & image);
		Image copy();
	};

	std::ostream& operator<<(std::ostream & os, const Image & image);
}