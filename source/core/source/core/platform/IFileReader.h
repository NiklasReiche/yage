#pragma once

#include <string>
#include <memory>

#include "IBinaryFile.h"
#include "ITextFile.h"

namespace platform
{
	class IFileReader
	{
	public:
		virtual std::unique_ptr<IBinaryFile> openBinaryFile(const std::string& filename, IFile::AccessMode mode) = 0;

		virtual std::unique_ptr<ITextFile> openTextFile(const std::string& filename, IFile::AccessMode mode) = 0;
	};
}
