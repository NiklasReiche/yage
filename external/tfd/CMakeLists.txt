add_library(tfd STATIC tinyfiledialogs.c)
target_include_directories(tfd
	INTERFACE include
	PRIVATE include/tfd
)
set_property(TARGET tfd PROPERTY POSITION_INDEPENDENT_CODE ON)
