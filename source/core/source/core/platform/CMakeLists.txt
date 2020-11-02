target_sources(yage_core
	PRIVATE
		File.h
		FileException.h
		FileLoader.h
		Window.h)

if(WIN32 OR UNIX)
	add_subdirectory(desktop)
elseif(ANDROID)
	add_subdirectory(android)
endif ()
