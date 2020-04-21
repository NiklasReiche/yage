#pragma once

#include <vector>

namespace gl
{
	class ITexture2D
	{
	public:
		virtual ~ITexture2D() = default;

		virtual void setData(
			int width, int height,
			std::vector<unsigned char>& data) = 0;
		
		virtual void setSubData(
			int xOffset, int yOffset,
			int width, int height,
			std::vector<unsigned char> & data) = 0;

		virtual void getData(
			std::vector<unsigned char> & data, 
			int level = 0) = 0;

		[[nodiscard]]
		virtual int getWidth() const = 0;
		[[nodiscard]]
		virtual int getHeight() const = 0;
		[[nodiscard]]
		virtual int getChannels() const = 0;
		[[nodiscard]]
		virtual bool isEmpty() const = 0;

	protected:
		ITexture2D() = default;
		ITexture2D(const ITexture2D& other) = default;
		ITexture2D(ITexture2D&& other) = default;
		ITexture2D& operator=(const ITexture2D & other) = default;
		ITexture2D& operator=(ITexture2D&& other) = default;
	};
}
