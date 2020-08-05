message(STATUS "Configuring yage_gl")

add_library(yage_gl)

target_include_directories(yage_gl
	INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(yage_gl
	PUBLIC opengl_libs
	PUBLIC glfw
	PUBLIC glad
	PUBLIC yage_gml
	)

target_sources(yage_gl

	INTERFACE gl/color.h
	INTERFACE gl/graphics.h

	INTERFACE gl/Context.h

	INTERFACE gl/PixelTransfer.h
	INTERFACE gl/ImageFormat.h
	INTERFACE gl/Texture2d.h
	INTERFACE gl/Cubemap.h
	INTERFACE gl/TextureCreator.h

	INTERFACE gl/Drawable.h
	INTERFACE gl/DrawableCreator.h

	INTERFACE gl/Shader.h
	INTERFACE gl/ShaderCreator.h

	INTERFACE gl/Frame.h
	INTERFACE gl/FrameCreator.h
	)

if (WIN32)
	target_link_libraries(yage_gl PUBLIC opengl32.lib)
	add_subdirectory(gl/opengl3)
elseif (UNIX)
	target_link_libraries(yage_gl PUBLIC libGL.so)
	add_subdirectory(gl/opengl3)
elseif (ANDROID)
	add_subdirectory(gl/opengles2)
endif ()

add_subdirectory(tests)
