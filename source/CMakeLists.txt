project(corelib)

file(GLOB corelib_SRC
    "core/graphics/*.cpp"
	"core/graphics/*.h"
	"core/graphics/GL3PLUS/*.cpp"
	"core/graphics/GL3PLUS/*.h"
	"core/graphics/GLES2/*.cpp"
	"core/graphics/GLES2/*.h"
	"core/graphics/GLES3/*.cpp"
	"core/graphics/GLES3/*.h"

	"core/math/*.cpp"
	"core/math/*.h"
)


#add_library(corelib STATIC ${corelib_SRC})
add_executable(testexe ${corelib_SRC})

target_link_libraries(testexe opengl32.lib glew32sMD_d.lib glfw32sMD_d.lib)