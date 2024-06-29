#pragma once

#include <string>

#include <core/gl/graphics.h>
#include <resource/Loader.h>
#include "font.h"
#include "core/platform/IFileReader.h"
#include "core/platform/IBinaryFile.h"

namespace yage::font
{
    class FontFileLoader : public res::Loader<Font>
	{
	public:
		explicit FontFileLoader(const std::shared_ptr<gl::ITextureCreator>& textureCreator,
                                const std::shared_ptr<platform::IFileReader>& fileReader) noexcept;

        Font load_resource(const std::string& uri) override;

	private:
        std::shared_ptr<gl::ITextureCreator> textureCreator;
        std::shared_ptr<platform::IFileReader> fileReader;
	};
}