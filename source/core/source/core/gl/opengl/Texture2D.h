#pragma once

#include "../Texture2D.h"
#include "Texture.h"
#include <vector>

namespace opengl
{
	class Texture2D final
		: public Texture, public gl::ITexture2D, public std::enable_shared_from_this<Texture2D>
	{
	public:
		~Texture2D() override = default;

		Texture2D(const Texture2D& other) = delete;

		Texture2D& operator=(const Texture2D& other) = delete;

		void setImage(const std::span<const unsigned char>& data) override;

		void setImage(const std::span<const unsigned char>& data, gl::PixelTransferParams params) override;

		void setSubImage(utils::Area subArea, const std::span<const unsigned char>& data) override;

		void setSubImage(
			utils::Area subArea,
			const std::span<const unsigned char>& data,
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

		using Texture::Texture;

		Texture2D(Texture2D&& other) noexcept;

		Texture2D& operator=(Texture2D&& other) noexcept;

		int getMaxMipmapLevel() const;

		friend class TextureCreator;
	};
}