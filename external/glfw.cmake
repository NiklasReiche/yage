###################################################################
# GLFW	                                                          #
###################################################################

option(BUILD_SHARED_LIBS "" OFF)

option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_VULKAN_STATIC "" OFF)

if(WIN32)
	option(USE_MSVC_RUNTIME_LIBRARY_DLL "" ON)
endif()

add_subdirectory(${YAGE_EXTERNAL_PATH}/glfw)
target_include_directories(glfw PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/glfw/include>)