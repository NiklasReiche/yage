###################################################################
# GLFW	                                                          #
###################################################################

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_VULKAN_STATIC "Use the Vulkan loader statically linked into application" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

if(WIN32)
	option(USE_MSVC_RUNTIME_LIBRARY_DLL "" ON)
endif()

add_subdirectory(${YAGE_EXTERNAL_PATH}/glfw)
#target_include_directories(glfw PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/glfw/include>)