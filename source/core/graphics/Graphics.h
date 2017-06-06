#pragma once

#ifdef GL3
#include "GL3PLUS\GL3_Context.h"
#endif // GL3

#ifdef GLES2
#include "GLES2\GLES2_Context.h"
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
#endif // GLES2
}