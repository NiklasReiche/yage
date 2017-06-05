#########################
# source/core/Cmake
#########################

file(GLOB graphics_SRC
    "graphics/*.cpp"
	"graphics/*.h"
)
file(GLOB graphics_GL3_SRC
	"graphics/GL3PLUS/*.cpp"
	"graphics/GL3PLUS/*.h"
)
file(GLOB graphics_GLES2_SRC
	"graphics/GLES2/*.cpp"
	"graphics/GLES2/*.h"
)
file(GLOB graphics_GLES3_SRC
	"graphics/GLES3/*.cpp"
	"graphics/GLES3/*.h"
)
file(GLOB platform_ANDROID_SRC
	"platform/android/*c"
	"platform/android/*.cpp"
	"platform/android/*.h"
)
file(GLOB input_SRC
	"input/*.cpp"
	"input/*.h"
)
file(GLOB math_SRC
	"math/*.cpp"
	"math/*.h"
)


if(WIN32)
	add_library(YAGEcore STATIC ${math_SRC}
								${input_SRC}
								${graphics_SRC}
								${graphics_GL3_SRC})

	target_link_libraries(YAGEcore PUBLIC opengl32.lib
								   PUBLIC glew32sMD_d.lib 
								   PUBLIC glfw32sMD_d.lib)

elseif(ANDROID)
	add_library(YAGEcore SHARED ${graphics_SRC} 
								${graphics_GLES2_SRC} 
								${platform_ANDROID_SRC})

	target_link_libraries(YAGEcore libEGL.so
								   libGLESv2.so)

endif()