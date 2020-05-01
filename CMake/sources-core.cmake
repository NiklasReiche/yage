#########################
# source/core/Cmake
#########################

set( YAGE_MATH_SOURCE

	"${YAGE_CORE_PATH}/math/maths.h"
	"${YAGE_CORE_PATH}/math/exception.h"

	"${YAGE_CORE_PATH}/math/vector/vectorbase.h"
	"${YAGE_CORE_PATH}/math/vector/vectorbase.tpp"
	"${YAGE_CORE_PATH}/math/vector/vec2.h"
	"${YAGE_CORE_PATH}/math/vector/vec2.tpp"
	"${YAGE_CORE_PATH}/math/vector/vec3.h"
	"${YAGE_CORE_PATH}/math/vector/vec3.tpp"
	"${YAGE_CORE_PATH}/math/vector/vec4.h"
	"${YAGE_CORE_PATH}/math/vector/vec4.tpp"

	"${YAGE_CORE_PATH}/math/matrix/matrixBase.h"
	"${YAGE_CORE_PATH}/math/matrix/matrixBase.tpp"
	"${YAGE_CORE_PATH}/math/matrix/matrixSpecial.h"
	"${YAGE_CORE_PATH}/math/matrix/mat2.h"
	"${YAGE_CORE_PATH}/math/matrix/mat2.tpp"
	"${YAGE_CORE_PATH}/math/matrix/mat3.h"
	"${YAGE_CORE_PATH}/math/matrix/mat3.tpp"
	"${YAGE_CORE_PATH}/math/matrix/mat4.h"
	"${YAGE_CORE_PATH}/math/matrix/mat4.tpp"

	"${YAGE_CORE_PATH}/math/vector.h"
	"${YAGE_CORE_PATH}/math/matrix.h"
	"${YAGE_CORE_PATH}/math/quaternion.h"
	"${YAGE_CORE_PATH}/math/quaternion.tpp"

	"${YAGE_CORE_PATH}/math/interpolation.h"
)

set( YAGE_GRAPHICS_SOURCE

	"${YAGE_CORE_PATH}/graphics/color.h"
	"${YAGE_CORE_PATH}/graphics/renderer.h"
	"${YAGE_CORE_PATH}/graphics/graphics.h"

	"${YAGE_CORE_PATH}/graphics/Context.h"
	"${YAGE_CORE_PATH}/graphics/Drawable.h"
	"${YAGE_CORE_PATH}/graphics/DrawableCreator.h"
	"${YAGE_CORE_PATH}/graphics/Texture2D.h"
	"${YAGE_CORE_PATH}/graphics/Cubemap.h"
	"${YAGE_CORE_PATH}/graphics/TextureCreator.h"
	"${YAGE_CORE_PATH}/graphics/Shader.h"
	"${YAGE_CORE_PATH}/graphics/ShaderCreator.h"
	"${YAGE_CORE_PATH}/graphics/Frame.h"
	"${YAGE_CORE_PATH}/graphics/FrameCreator.h"

)
set( YAGE_GRAPHICS_GL3_SOURCE

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Exception.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Enum.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_BaseObject.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Object.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Object.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Drawable.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Drawable.cpp"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_DrawableCreator.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_DrawableCreator.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Framebuffer.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Framebuffer.cpp"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_FrameCreator.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_FrameCreator.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Shader.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Shader.cpp"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_ShaderCreator.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_ShaderCreator.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Texture.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Texture.cpp"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Texture2D.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Texture2D.cpp"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Cubemap.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Cubemap.cpp"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_TextureCreator.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_TextureCreator.cpp"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_UnitShader.h"

	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Context.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Context.cpp"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Renderer.h"
	"${YAGE_CORE_PATH}/graphics/GL3/GL3_Renderer.cpp"

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
	"${YAGE_CORE_PATH}/graphics/GLES2/GLES2_Framebuffer.cpp"

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

	"${YAGE_CORE_PATH}/input/InputListener.h"

	"${YAGE_CORE_PATH}/input/Event.h"
	"${YAGE_CORE_PATH}/input/Event.cpp"
	"${YAGE_CORE_PATH}/input/KeyEvent.h"
	"${YAGE_CORE_PATH}/input/KeyEvent.cpp"
	"${YAGE_CORE_PATH}/input/MousePosEvent.h"
	"${YAGE_CORE_PATH}/input/MousePosEvent.cpp"
	"${YAGE_CORE_PATH}/input/MouseWheelEvent.h"
	"${YAGE_CORE_PATH}/input/MouseWheelEvent.cpp"

)

set( YAGE_PLATFORM_SOURCE

	"${YAGE_CORE_PATH}/platform/OpenGL.h"

	"${YAGE_CORE_PATH}/platform/Window.h"
	"${YAGE_CORE_PATH}/platform/File.h"
	"${YAGE_CORE_PATH}/platform/FileLoader.h"
	"${YAGE_CORE_PATH}/platform/FileLoader.cpp"

	"${YAGE_CORE_PATH}/platform/FileException.h"
	
)
set( YAGE_PLATFORM_DESKTOP_SOURCE

	"${YAGE_CORE_PATH}/platform/desktop/GlfwWindow.h"
	"${YAGE_CORE_PATH}/platform/desktop/GlfwWindow.cpp"
	"${YAGE_CORE_PATH}/platform/desktop/GlfwException.h"
	"${YAGE_CORE_PATH}/platform/desktop/File.h"
	"${YAGE_CORE_PATH}/platform/desktop/File.cpp"

)
set( YAGE_PLATFORM_ANDROID_SOURCE

	"${YAGE_CORE_PATH}/platform/android/Android_Handle.h"
	"${YAGE_CORE_PATH}/platform/android/Android_Handle.cpp"
	"${YAGE_CORE_PATH}/platform/android/android_native_app_glue.h"
	"${YAGE_CORE_PATH}/platform/android/android_native_app_glue.c"
	"${YAGE_CORE_PATH}/platform/android/Android_File.h"
	"${YAGE_CORE_PATH}/platform/android/Android_File.cpp"

)

set( YAGE_UTILS_SOURCE

	"${YAGE_CORE_PATH}/utils/utils.h"
	"${YAGE_CORE_PATH}/utils/NotImplementedException"

)