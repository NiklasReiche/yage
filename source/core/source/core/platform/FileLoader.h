#pragma once

#include <string>

#include "File.h"

namespace platform
{
	class FileLoader
	{
	public:
		static File openFile(
			const std::string & filename, 
			IFile::AccessMode mode = IFile::AccessMode::READ)
		{
#ifdef DESKTOP
			return std::make_unique<desktop::File>(filename, mode);
#endif
		}
	};
}
