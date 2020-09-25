#pragma once

#include <stdexcept>
#include "OpenGL.h"
#include "../ImageFormat.h"

namespace gl3
{
	enum class InternalFormat
	{
		UNDEFINED,
		R8 = GL_R8,
		RG8 = GL_RG8,
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8
	};

	enum class ImageFormat
	{
		UNDEFINED,
		R = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA
	};

	enum class PixelType
	{
		UNDEFINED,
		BYTE = GL_BYTE,
		U_BYTE = GL_UNSIGNED_BYTE,
		SHORT = GL_SHORT,
		U_SHORT = GL_UNSIGNED_SHORT,
		INT = GL_INT,
		U_INT = GL_UNSIGNED_INT,
		FLOAT = GL_FLOAT,
	};

	enum class TextureType
	{
		UNDEFINED,
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_CUBEMAP = GL_TEXTURE_CUBE_MAP,
	};

	enum class TextureWrapper
	{
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
		REPEAT = GL_REPEAT,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT
	};

	enum class TextureFilter
	{
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum class MipmapOption
	{
		NEAREST_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class DrawMode
	{
		DRAW_STATIC = GL_STATIC_DRAW,
		DRAW_DYNAMIC = GL_DYNAMIC_DRAW,
		DRAW_STREAM = GL_STREAM_DRAW
	};

	enum class BufferType
	{
		BUFFER_COLOR = GL_COLOR_BUFFER_BIT,
		BUFFER_DEPTH = GL_DEPTH_BUFFER_BIT,
		BUFFER_STENCIL = GL_STENCIL_BUFFER_BIT
	};

	enum class RenderFlag
	{
		DEPTH_TEST = GL_DEPTH_TEST,
		STENCIL_TEST = GL_STENCIL_TEST,
		BLENDING = GL_BLEND,
		WIREFRAME
	};

	enum class PrimitiveType
	{
		TRIANGLES = GL_TRIANGLES
	};

	inline InternalFormat convertToInternalFormat(const gl::ImageFormat format)
	{
		switch (format)
		{
		case gl::ImageFormat::R:
			return InternalFormat::R8;
		case gl::ImageFormat::RG:
			return InternalFormat::RG8;
		case gl::ImageFormat::RGB:
			return InternalFormat::RGB8;
		case gl::ImageFormat::RGBA:
			return InternalFormat::RGBA8;
		default:
			return InternalFormat::UNDEFINED;
		}
	}
	
	inline ImageFormat convertToImageFormat(const gl::ImageFormat format)
	{
		switch (format)
		{
		case gl::ImageFormat::R:
			return ImageFormat::R;
		case gl::ImageFormat::RG:
			return ImageFormat::RG;
		case gl::ImageFormat::RGB:
			return ImageFormat::RGB;
		case gl::ImageFormat::RGBA:
			return ImageFormat::RGBA;
		default:
			return ImageFormat::UNDEFINED;
		}
	}

	inline gl::ImageFormat convertToTextureFormat(const InternalFormat format)
	{
		switch (format)
		{
		case InternalFormat::R8:
			return gl::ImageFormat::R;
		case InternalFormat::RG8:
			return gl::ImageFormat::RG;
		case InternalFormat::RGB8:
			return gl::ImageFormat::RGB;
		case InternalFormat::RGBA8:
			return gl::ImageFormat::RGBA;
		default:
			throw std::invalid_argument("unknown image format");
		}
	}
}
