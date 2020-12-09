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
		[[nodiscard]]
		virtual std::unique_ptr<IBinaryFile> openBinaryFile(const std::string& filename, IFile::AccessMode mode) const = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ITextFile> openTextFile(const std::string& filename, IFile::AccessMode mode) const = 0;
	};
}
