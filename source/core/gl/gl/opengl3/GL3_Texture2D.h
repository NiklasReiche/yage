#pragma once

#include <vector>
#include "../Texture2D.h"
#include "GL3_Texture.h"

namespace gl3
{
	class GL3_Texture2D final
		: public GL3_Texture, public gl::ITexture2D, public std::enable_shared_from_this<GL3_Texture2D>
	{
	public:
		~GL3_Texture2D() override = default;

		GL3_Texture2D(const GL3_Texture2D &other) = delete;

		GL3_Texture2D &operator=(const GL3_Texture2D &other) = delete;

		void setImage(const std::span<unsigned char>& data) override;

		void setImage(const std::span<unsigned char>& data, gl::PixelTransferParams params) override;

		void setSubImage(utils::Area subArea, const std::span<unsigned char>& data) override;

		void setSubImage(
			utils::Area subArea,
		    const std::span<unsigned char>& data,
			gl::PixelTransferParams params) override;

		std::vector<unsigned char> getImage() override;

		std::vector<unsigned char> getImage(gl::PixelTransferParams params) override;

		std::vector<unsigned char> getMipmapImage(int level) override;

		std::vector<unsigned char> getMipmapImage(int level, gl::PixelTransferParams params) override;

		void generateMipmaps() override;

		int getWidth() const override;

		int getHeight() const override;

		int getChannels() const override;

		gl::ImageFormat getFormat() const override;

	private:
		int width = 0;
		int height = 0;
		int maxMipmapLevel = 0;

		using GL3_Texture::GL3_Texture;

		GL3_Texture2D(GL3_Texture2D &&other) noexcept;

		GL3_Texture2D &operator=(GL3_Texture2D &&other) noexcept;

		int getMaxMipmapLevel() const;

		friend class GL3_TextureCreator;
	};
}