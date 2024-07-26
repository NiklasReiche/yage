target_sources(yage_core
	PRIVATE
		IFile.h
		FileException.h
		IFileReader.h
		ITextFile.h
		IBinaryFile.h
        IWindow.h)

if(WIN32 OR UNIX)
	add_subdirectory(desktop)
elseif(ANDROID)
	add_subdirectory(android)
endif ()
