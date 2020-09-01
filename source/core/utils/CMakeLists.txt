add_library(yage_utils INTERFACE)

target_include_directories(yage_utils
	INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(yage_utils
	INTERFACE utils/utils.h
	INTERFACE utils/Point.h
	INTERFACE utils/NotImplementedException.h
	)