#pragma once

#include <vector>
#include "../Texture2D.h"
#include "GL3_Texture.h"

namespace gl3
{
	class GL3_Texture2D final : public GL3_Texture, public gl::ITexture2D, public std::enable_shared_from_this<GL3_Texture2D>
	{
	public:
		virtual ~GL3_Texture2D() = default;
		GL3_Texture2D(const GL3_Texture2D& other) = delete;
		GL3_Texture2D& operator=(const GL3_Texture2D& other) = delete;

		void setData(
			int width, int height,
			unsigned char* data);
		void setData(
			int width, int height,
			std::vector<unsigned char>& data) override;
		
		void setSubData(
			int xOffset, int yOffset,
			int width, int height,
			unsigned char* data);
		void setSubData(
			int xOffset, int yOffset,
			int width, int height,
			std::vector<unsigned char>& data) override;

		void getData(
			std::vector<unsigned char>& data,
			int level = 0) override;

		int getWidth() const override;
		int getHeight() const override;
		int getChannels() const override;
		bool isEmpty() const override;

	private:
		int width = 0;
		int height = 0;

		using GL3_Texture::GL3_Texture;

		GL3_Texture2D(GL3_Texture2D&& other) noexcept;
		GL3_Texture2D& operator=(GL3_Texture2D&& other) noexcept;

		friend class GL3_TextureCreator;
	};
}