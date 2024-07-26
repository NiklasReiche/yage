#include "TextFile.h"

namespace yage::platform::desktop
{
	TextFile::TextFile(const std::string& filename, AccessMode mode)
		: File(filename, FileType::TEXT, mode)
	{
	}

	std::string TextFile::read()
	{
		std::string output;
		*fileStream >> output;
		return output;
	}

	std::string TextFile::read_line()
	{
		std::string output;
		std::getline(*fileStream, output);
		return output;
	}

	std::stringstream& TextFile::read_all(std::stringstream& output)
	{
		output << fileStream->rdbuf();
		return output;
	}

	std::string TextFile::read_all()
	{
		std::stringstream output;
		return read_all(output).str();
	}
}
