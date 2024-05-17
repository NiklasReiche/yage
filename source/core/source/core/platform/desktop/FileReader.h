#pragma once

#include <memory>

#include <core/platform/IFileReader.h>

namespace platform::desktop
{
	class FileReader : public IFileReader
	{
	public:
        FileReader() = default;
        virtual ~FileReader() = default;

		[[nodiscard]]
		std::unique_ptr<IBinaryFile> openBinaryFile(const std::string& filename, IFile::AccessMode mode) const override;

		[[nodiscard]]
		std::unique_ptr<ITextFile> openTextFile(const std::string &filename, IFile::AccessMode mode) const override;
	};
}
