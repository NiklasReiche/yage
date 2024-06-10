#include "File.h"

namespace yage::platform::desktop
{
	File::File(const std::string & filename, FileType type, const IFile::AccessMode mode)
		: filename(filename), mode(mode)
	{
		switch (mode)
		{
		case AccessMode::READ:
			fileStream = std::make_unique<std::fstream>(filename,
											   type == FileType::TEXT ? std::ios::in :
											   std::ios::in | std::ios::binary);
			break;
		case AccessMode::WRITE:
			fileStream = std::make_unique<std::fstream>(filename,
											   type == FileType::TEXT ? std::ios::out :
											   std::ios::out | std::ios::binary);
			break;
		case AccessMode::READ_WRITE:
			fileStream = std::make_unique<std::fstream>(filename,
											   type == FileType::TEXT ? std::ios::in | std::ios::out :
											   std::ios::in | std::ios::out | std::ios::binary);
			break;
		}

		if (!fileStream->is_open()){
			throw FileOpenException(filename);
		}
	}

	void File::seek(const int index, const IFile::SeekOffset offset)
	{
		if(!fileStream->seekg(index, std::ios_base::seekdir(convertSeekOffset(offset)))) {
			throw FileSeekException(filename);
		}
	}

	std::string File::getFileName()
	{
		return filename;
	}

	int File::convertSeekOffset(const IFile::SeekOffset offset)
	{
		switch(offset)
		{
		case IFile::SeekOffset::BEG:
			return std::ios::beg;
		case IFile::SeekOffset::CUR:
			return std::ios::cur;
		default:
			throw std::invalid_argument("The given option is not supported");
		}
	}

	int File::convertAccessMode(const IFile::AccessMode mode)
	{
		switch (mode)
		{
		case IFile::AccessMode::READ:
			return std::ios::in;
		case IFile::AccessMode::WRITE:
			return std::ios::out;
		case IFile::AccessMode::READ_WRITE:
			return std::ios::in | std::ios::out;
		default:
			throw std::invalid_argument("The given option is not supported");
		}
	}

	bool File::eof()
	{
		fileStream->peek();
		return fileStream->eof();
	}
}