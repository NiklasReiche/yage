#pragma once

#include <string>
#include <memory>

#include "IBinaryFile.h"
#include "ITextFile.h"

namespace yage::platform
{
	class IFileReader
	{
	public:
        virtual ~IFileReader() = default;

		[[nodiscard]]
		virtual std::unique_ptr<IBinaryFile> openBinaryFile(const std::string& filename, IFile::AccessMode mode) const = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ITextFile> openTextFile(const std::string& filename, IFile::AccessMode mode) const = 0;
	};
}
