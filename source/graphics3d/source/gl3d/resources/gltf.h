#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include <gl3d/material.h>

namespace gl3d::resources
{
	std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material> readGltf(
		const platform::IFileReader& fileReader, const std::string& filename,
		gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator);

#if 0
	template<typename T>
	std::vector<T> readBufferViewInteger(
		nlohmann::json data, int i, int offset, int nElements,
		std::vector<std::unique_ptr<platform::IBinaryFile>> buffers)
	{
		std::vector<T> elements(nElements);

		int bufferIndex = data["bufferView"][i]["buffer"].get<int>();
		int byteOffset = offset + data["bufferView"][i]["byteOffset"].get<int>();

		buffers[bufferIndex]->seek(byteOffset, platform::IFile::SeekOffset::BEG);
		buffers[bufferIndex]->read(elements.data(), nElements * sizeof(T));

		return elements;
	}
#endif
}
