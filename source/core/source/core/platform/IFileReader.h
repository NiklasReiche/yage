#pragma once

#include <string>
#include <memory>

#include "IBinaryFile.h"

namespace platform
{
	class IFileReader
	{
	public:
		virtual std::unique_ptr<IBinaryFile> openBinaryFile(const std::string & filename, IFile::AccessMode mode) = 0;
	};
}
