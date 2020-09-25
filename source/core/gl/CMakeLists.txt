message(STATUS "Configuring yage_gl")

add_library(yage_gl STATIC)

target_compile_options(yage_gl
	PRIVATE -fdiagnostics-color=always -Wall -Wextra -Werror)

target_compile_definitions(yage_platform
	PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

target_include_directories(yage_gl
	PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(yage_gl
	PUBLIC
		yage_platform
		yage_gml
	)

target_sources(yage_gl
	PRIVATE
		gl/color.h
		gl/graphics.h

		gl/Context.h

		gl/PixelTransfer.h
		gl/ImageFormat.h
		gl/Texture2D.h
		gl/Cubemap.h
		gl/TextureCreator.h

		gl/Drawable.h
		gl/DrawableCreator.h

		gl/Shader.h
		gl/ShaderCreator.h

		gl/Frame.h
		gl/FrameCreator.h
	)

if (WIN32)
	target_link_libraries(yage_gl PUBLIC opengl32.lib)
	add_subdirectory(gl/opengl3)
elseif (UNIX)
	target_link_libraries(yage_gl PUBLIC libGL.so)
	add_subdirectory(gl/opengl3)
elseif (ANDROID)

endif ()

add_subdirectory(tests)
