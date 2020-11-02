add_library(tfd tinyfiledialogs.c)
target_include_directories(tfd
	INTERFACE include
	PRIVATE include/tfd
)
