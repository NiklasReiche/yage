#pragma once

#include <span>

#include "ImageFormat.h"

namespace gl
{
	enum class RowAlignment
	{
		B_1 = 1,
		B_2 = 2,
		B_4 = 4,
		B_8 = 8
	};
	
	struct PixelTransferParams
	{
		ImageFormat dataFormat = ImageFormat::RGBA;
		RowAlignment rowAlignment = RowAlignment::B_1;
	};

	inline int calculatePadding(
		const int alignment, 
		const int width, 
		const int size)
	{
		return (alignment - (width * size) % alignment) % alignment;
	}

	inline int calculateImageDataSize(
		const int width,
		const int height,
		const int channels,
		const int alignment)
	{
		const int padding = calculatePadding(alignment, width, channels);
		return width * height * channels + height * padding;
	}

	inline bool isDataSizeCorrect(
		const int width,
		const int height,
		const int channels,
		const int alignment,
		const std::span<unsigned char>& data)
	{
		return data.size() == static_cast<std::span<unsigned char>::size_type>(
			calculateImageDataSize(width, height, channels, alignment));
	}
}
