#include "BinaryFile.h"
#include <core/platform/FileException.h>

namespace platform::desktop
{
	BinaryFile::BinaryFile(const std::string& filename, AccessMode mode)
		: File(filename, FileType::BINARY, mode)
	{ }

	void BinaryFile::read(void* buffer, size_t size)
	{
		if (mode == AccessMode::READ || mode == AccessMode::READ_WRITE) {
			fileStream->read(static_cast<char*>(buffer), size);
		} else {
			throw FileException(FileException::ErrorCode::ACCESS_VIOLATION, "", filename);
		}
	}

	void BinaryFile::write(void* buffer, size_t size)
	{
		if (mode == AccessMode::WRITE || mode == AccessMode::READ_WRITE) {
			fileStream->write((char*)buffer, size);
		} else {
			throw FileException(FileException::ErrorCode::ACCESS_VIOLATION, "", filename);
		}
	}
}
