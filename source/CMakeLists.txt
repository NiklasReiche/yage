file(GLOB YAGEcore_SRC
    "core/graphics/*.cpp"
	"core/graphics/*.h"
	"core/input/*.cpp"
	"core/input/*.h"
	"core/math/*.cpp"
	"core/math/*.h"
)
file(GLOB YAGEcore_GL3_SRC
	"core/graphics/GL3PLUS/*.cpp"
	"core/graphics/GL3PLUS/*.h"
)
file(GLOB YAGEcore_GLES2_SRC
	"core/graphics/GLES2/*.cpp"
	"core/graphics/GLES2/*.h"
)
file(GLOB YAGEcore_GLES3_SRC
	"core/graphics/GLES3/*.cpp"
	"core/graphics/GLES3/*.h"
)
file(GLOB YAGEcore_ANDROID_SRC
	"core/platform/android/*c"
	"core/platform/android/*.cpp"
	"core/platform/android/*.h"
)

if(ANDROID)
	set(YAGEcore_LIBS libEGL.so
					  libGLESv2.so)
elseif(WIN32)
	set(YAGEcore_LIBS opengl32.lib 
					  glew32sMD_d.lib 
					  glfw32sMD_d.lib)
endif()


if(ANDROID)
	add_library(YAGEcore SHARED ${YAGEcore_SRC} 
								${YAGEcore_GLES2_SRC} 
								${YAGEcore_ANDROID_SRC})
	target_link_libraries(YAGEcore ${YAGEcore_LIBS})
elseif(WIN32)
	add_library(YAGEcore STATIC ${YAGEcore_SRC}
								${YAGEcore_GL3_SRC})
	target_link_libraries(YAGEcore ${YAGEcore_LIBS})
endif()