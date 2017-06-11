#########################
# source/core/Cmake
#########################

file(GLOB math_SRC
	"math/*.cpp"
	"math/*.h"
)

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

file(GLOB platform_SRC
	"platform/*.cpp"
	"platform/*.h"
)
file(GLOB platform_ANDROID_SRC
	"platform/android/*c"
	"platform/android/*.cpp"
	"platform/android/*.h"
)
file(GLOB platform_DESKTOP_SRC
	"platform/desktop/*.cpp"
	"platform/desktop/*.h"
)

file(GLOB input_SRC
	"input/*.cpp"
	"input/*.h"
)