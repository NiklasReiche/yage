#include "FileReader.h"
#include "BinaryFile.h"
#include "TextFile.h"

namespace yage::platform::desktop
{
	std::unique_ptr<IBinaryFile> FileReader::open_binary_file(const std::string& filename, IFile::AccessMode mode) const
	{
		return std::make_unique<desktop::BinaryFile>(filename, mode);
	}

	std::unique_ptr<ITextFile> FileReader::open_text_file(const std::string& filename, IFile::AccessMode mode) const
	{
		return std::make_unique<desktop::TextFile>(filename, mode);
	}
}
