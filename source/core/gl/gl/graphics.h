#pragma once

#include "Context.h"
#include "Renderer.h"

#include "Drawable.h"
#include "DrawableCreator.h"

#include "Frame.h"
#include "FrameCreator.h"

#include "Shader.h"
#include "ShaderCreator.h"

#include "Texture2D.h"
#include "Cubemap.h"
#include "TextureCreator.h"

#include "color.h"
#include "ImageFormat.h"

#ifdef GL3
#include "GL3/GL3_Exception.h"
#include "GL3/GL3_Enum.h"

#include "GL3/GL3_Context.h"
#include "GL3/GL3_Renderer.h"

#include "GL3/GL3_Drawable.h"
#include "GL3/GL3_DrawableCreator.h"

#include "GL3/GL3_FrameBuffer.h"
#include "GL3/GL3_FrameCreator.h"

#include "GL3/GL3_Shader.h"
#include "GL3/GL3_ShaderCreator.h"

#include "GL3/GL3_Texture.h"
#include "GL3/GL3_Cubemap.h"
#include "GL3/GL3_TextureCreator.h"

#endif

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
#endif
