#pragma once

#ifdef GL3
#include "GL3/gl3.h"
#include "GL3/GL3_Exception.h"
#include "GL3/GL3_Enum.h"

#include "GL3/GL3_Drawable.h"
#include "GL3/GL3_Texture.h"
#include "GL3/GL3_Framebuffer.h"
#include "GL3/GL3_Shader.h"
#include "GL3/GL3_Viewport.h"

#include "GL3/GL3_Context.h"
#include "GL3/GL3_ShaderLoader.h"
#endif // GL3

#ifdef GLES2
#include "GLES2/gles2.h"

#include "GLES2/GLES2_Enum.h"
#include "GLES2/GLES2_Drawable.h"
#include "GLES2/GLES2_Texture.h"
#include "GLES2/GLES2_Framebuffer.h"
#include "GLES2/GLES2_Shader.h"
#include "GLES2/GLES2_Viewport.h"

#include "GLES2/GLES2_Context.h"
#include "GLES2/GLES2_ShaderLoader.h"
#endif // GLES2

namespace gl
{
#ifdef GL3
	typedef GLshort Gshort;
	typedef GLushort Gushort;
	typedef GLint Gint;
	typedef GLuint Guint;
	typedef GLfloat Gfloat;
	typedef GLchar Gchar;
	typedef GLenum Genum;

	typedef gl3::GL3_Context GraphicsContext;
	typedef gl3::GL3_Drawable Drawable;
	typedef gl3::GL3_Shader Shader;
	typedef gl3::GL3_Texture Texture;
	typedef gl3::GL3_Framebuffer Framebuffer;
	typedef gl3::Viewport Viewport;
	typedef gl3::GL3_ShaderLoader ShaderLoader;

	typedef gl3::ImageFormat ImageFormat;
	typedef gl3::InternalFormat InternalFormat;
	typedef gl3::PixelType PixelType;
	typedef gl3::TextureType TextureType;
	typedef gl3::TextureFilter TextureFilter;
	typedef gl3::MipmapOption MipmapOption;
	typedef gl3::VertexFormat VertexFormat;
	typedef gl3::DrawMode DrawMode;
	typedef gl3::BufferType BufferType;
	typedef gl3::RenderFlag RenderFlag;
	typedef gl3::PrimitiveType PrimitiveType;
	typedef gl3::TextureWrapper TextureWrapper;
#endif // GL3

#ifdef GLES2
	typedef GLshort Gshort;
	typedef GLushort Gushort;
	typedef GLint Gint;
	typedef GLuint Guint;
	typedef GLfloat Gfloat;
	typedef GLchar Gchar;
	typedef GLenum Genum;

	typedef gles2::GLES2_Context GraphicsContext;
	typedef gles2::GLES2_Drawable Drawable;
	typedef gles2::GLES2_Shader Shader;
	typedef gles2::GLES2_Texture Texture;
	typedef gles2::GLES2_Framebuffer Framebuffer;
	typedef gles2::GLES2_Viewport Viewport;
	typedef gles2::GLES2_ShaderLoader ShaderLoader;

	typedef gles2::ImageFormat ImageFormat;
	typedef gles2::InternalFormat InternalFormat;
	typedef gles2::PixelType PixelType;
	typedef gles2::TextureType TextureType;
	typedef gles2::TextureFilter TextureFilter;
	typedef gles2::MipmapOption MipmapOption;
	typedef gles2::VertexFormat VertexFormat;
	typedef gles2::DrawMode DrawMode;
	typedef gles2::BufferType BufferType;
	typedef gles2::RenderFlag RenderFlag;
	typedef gles2::PrimitiveType PrimitiveType;
	typedef gles2::TextureWrapper TextureWrapper;
#endif // GLES2
}