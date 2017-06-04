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