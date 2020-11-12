#include "TextFile.h"

namespace platform::desktop
{
	TextFile::TextFile(const std::string& filename, AccessMode mode)
		: File(filename, FileType::TEXT, mode)
	{

	}
}
