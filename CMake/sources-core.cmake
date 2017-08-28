#########################
# source/core/Cmake
#########################

set( YAGE_MATH_SOURCE

	"${YAGE_CORE_PATH}/math/MVector.h"
	"${YAGE_CORE_PATH}/math/Interpolation.h"
	
)

set( YAGE_GRAPHICS_SOURCE

	"${YAGE_CORE_PATH}/graphics/gl.h"
	"${YAGE_CORE_PATH}/graphics/Color.h"
	"${YAGE_CORE_PATH}/graphics/Renderer.h"
	"${YAGE_CORE_PATH}/graphics/Graphics.h"

)
set( YAGE_GRAPHICS_GL3_SOURCE

	"${YAGE_CORE_PATH}/graphics/GL3/gl3.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Exception.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Enum.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Object.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Object.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Drawable.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Drawable.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Framebuffer.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Framebuffer.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Shader.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Shader.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Texture.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Texture.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Viewport.h"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_UnitShader.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_State.h"	
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Context.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Context.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_ShaderLoader.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_ShaderLoader.cpp"

)
set( YAGE_GRAPHICS_GLES2_SOURCE

	"${YAGE_CORE_PATH}/graphics/GLES2/gles2.h"
	
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Enum.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_State.h"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Object.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Object.cpp"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Drawable.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Drawable.cpp"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Framebuffer.h"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Shader.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Shader.cpp"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Texture.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Texture.cpp"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Viewport.h"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_UnitShader.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Context.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Context.cpp"

	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_ShaderLoader.h"
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_ShaderLoader.cpp"

)

set( YAGE_INPUT_SOURCE

	"${YAGE_CORE_PATH}/input/InputNames.h"
	"${YAGE_CORE_PATH}/input/InputState.h"
	"${YAGE_CORE_PATH}/input/InputController.h"
	"${YAGE_CORE_PATH}/input/InputController.cpp"
	"${YAGE_CORE_PATH}/input/InputListener.h"
	"${YAGE_CORE_PATH}/input/InputListener.cpp"

)

set( YAGE_PLATFORM_SOURCE

	"${YAGE_CORE_PATH}/platform/Platform.h"
	"${YAGE_CORE_PATH}/platform/GenericHandle.h"
	"${YAGE_CORE_PATH}/platform/clib.h"
	"${YAGE_CORE_PATH}/platform/Exception.h"

)
set( YAGE_PLATFORM_DESKTOP_SOURCE

	"${YAGE_CORE_PATH}/platform/desktop/GLFW_Callback.h"
	"${YAGE_CORE_PATH}/platform/desktop/GLFW_Callback.cpp"
	"${YAGE_CORE_PATH}/platform/desktop/GLFW_Handle.h"
	"${YAGE_CORE_PATH}/platform/desktop/GLFW_Handle.cpp"
	"${YAGE_CORE_PATH}/platform/desktop/Desktop_File.h"
	"${YAGE_CORE_PATH}/platform/desktop/Desktop_File.cpp"

)
set( YAGE_PLATFORM_ANDROID_SOURCE

	"${YAGE_CORE_PATH}/platform/android/Android_Handle.h"
	"${YAGE_CORE_PATH}/platform/android/Android_Handle.cpp"
	"${YAGE_CORE_PATH}/platform/android/android_native_app_glue.h"
	"${YAGE_CORE_PATH}/platform/android/android_native_app_glue.c"
	"${YAGE_CORE_PATH}/platform/android/Android_File.h"
	"${YAGE_CORE_PATH}/platform/android/Android_File.cpp"

)