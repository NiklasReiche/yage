#include "FileReader.h"
#include "BinaryFile.h"

namespace platform::desktop
{
	std::unique_ptr<IBinaryFile> FileReader::openBinaryFile(const std::string& filename, IFile::AccessMode mode)
	{
		return std::make_unique<desktop::BinaryFile>(filename, mode);
	}
}
