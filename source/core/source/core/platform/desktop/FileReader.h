#pragma once

#include <memory>

#include <core/platform/IFileReader.h>

namespace platform::desktop
{
	class FileReader : public IFileReader
	{
	public:
		std::unique_ptr<IBinaryFile> openBinaryFile(const std::string& filename, IFile::AccessMode mode) override;
	};
}
