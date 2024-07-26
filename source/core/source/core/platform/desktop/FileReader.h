#pragma once

#include <memory>

#include <core/platform/IFileReader.h>

namespace yage::platform::desktop
{
	class FileReader : public IFileReader
	{
	public:
        FileReader() = default;
        virtual ~FileReader() = default;

		[[nodiscard]]
		std::unique_ptr<IBinaryFile> open_binary_file(const std::string& filename, IFile::AccessMode mode) const override;

		[[nodiscard]]
		std::unique_ptr<ITextFile> open_text_file(const std::string &filename, IFile::AccessMode mode) const override;
	};
}
