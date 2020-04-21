#include "FileLoader.h"
#include "desktop/File.h"

namespace sys
{
	File FileLoader::openFile(
		const std::string & filename, 
		IFile::AccessMode mode)
	{
#ifdef DESKTOP
		return std::make_unique<desktop::File>(filename, mode);
#endif
	}
}
