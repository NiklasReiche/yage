#pragma once

#include <vector>

namespace gl
{
	class ITexture3D
	{
	public:
		virtual ~ITexture3D() = default;
		
		virtual void setSubData(
			int xOffset, int yOffset, int zOffset,
			int width, int height, int depth,
			std::vector<unsigned char> & data) = 0;

		virtual void setData(
			int width, int height, int depth, 
			std::vector<unsigned char> & data) = 0;

		virtual void getData(std::vector<unsigned char> & data) = 0;

		[[nodiscard]]
		virtual int getWidth() const = 0;
		[[nodiscard]]
		virtual int getHeight() const = 0;
		[[nodiscard]]
		virtual int getDepth() const = 0;
		[[nodiscard]]
		virtual int getChannels() const = 0;

	protected:
		ITexture3D() = default;
		ITexture3D(const ITexture3D& other) = default;
		ITexture3D(ITexture3D&& other) = default;
		ITexture3D& operator=(const ITexture3D& other) = default;
		ITexture3D& operator=(ITexture3D&& other) = default;
	};
}
