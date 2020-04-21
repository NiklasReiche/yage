#include "File.h"
#include "utils/NotImplementedException.h"

namespace sys::desktop
{
	File::File(const std::string & filename, const IFile::AccessMode mode)
		: path(filename), mode(mode)
	{
		switch (mode)
		{
		case AccessMode::READ:
			fileStream = std::make_unique<std::fstream>(filename, std::ios::in | std::ios::binary);
			break;
		case AccessMode::WRITE:
			fileStream = std::make_unique<std::fstream>(filename, std::ios::out | std::ios::binary);
			break;
		case AccessMode::READ_WRITE:
			fileStream = std::make_unique<std::fstream>(filename, std::ios::in | std::ios::out | std::ios::binary);
			break;
		}
	}
	
	File::File(File&& other) noexcept
		: path(std::move(other.path)), fileStream(std::move(other.fileStream)), mode(other.mode)
	{
		other.path = "";
	}
	
	File& File::operator=(File&& other) noexcept
	{
		if (this == &other)
			return *this;
		
		this->fileStream = std::move(other.fileStream);
		this->mode = other.mode;
		this->path = other.path;
		return *this;
	}

	void File::seek(const int index, const IFile::SeekOffset offset)
	{
		fileStream->seekg(index, std::ios_base::seekdir(convertSeekOffset(offset)));
	}

	void File::read(void* buffer, const size_t size)
	{
		if (mode == AccessMode::READ || mode == AccessMode::READ_WRITE) {
			fileStream->read(static_cast<char*>(buffer), size);
		}
		else {
			throw FileException(FileException::ErrorCode::ACCESS_VIOLATION, "", path);
		}
	}
	void File::read(std::stringstream & output)
	{
		std::streampos fileLength = 0;
		fileStream->seekg(0, std::ios::beg);
		fileLength = fileStream->tellg();
		fileStream->seekg(0, std::ios::end);
		fileLength = fileStream->tellg() - fileLength;

		// Allocate memory to read your file
		char* fileContent = new char[(size_t)fileLength + 1];
		// Read your file
		fileStream->seekg(0, std::ios::beg);
		fileStream->read(fileContent, fileLength);
		// For safety you can add a 0 terminating character at the end of your file ...
		fileContent[fileLength] = '\0';

		// Do whatever you want with the content of the file
		output = std::stringstream(fileContent);

		// Free the memory you allocated earlier
		delete[] fileContent;
	}

	void File::write(void* buffer, const size_t size)
	{
		if (mode == AccessMode::WRITE || mode == AccessMode::READ_WRITE) {
			fileStream->write((char*)buffer, size);
		}
		else {
			throw FileException(FileException::ErrorCode::ACCESS_VIOLATION, "", path);
		}
	}

	void File::write(std::stringstream & data)
	{
		throw NotImplementedException();
	}

	void File::close()
	{
		fileStream->close();
	}

	bool File::isOpen()
	{
		return fileStream->is_open();
	}

	bool File::getError() const
	{
		return (fileStream->eof() || fileStream->bad() || fileStream->fail());
	}

	SeekOffset File::convertSeekOffset(const IFile::SeekOffset offset)
	{
		switch(offset)
		{
		case IFile::SeekOffset::BEG:
			return desktop::SeekOffset::BEG;
		case IFile::SeekOffset::CUR:
			return desktop::SeekOffset::CUR;
		default:
			throw std::invalid_argument("The given option is not supported");
		}
	}

	AccessMode File::convertAccessMode(const IFile::AccessMode mode)
	{
		switch (mode)
		{
		case IFile::AccessMode::READ:
			return desktop::AccessMode::READ;
		case IFile::AccessMode::WRITE:
			return desktop::AccessMode::WRITE;
		case IFile::AccessMode::READ_WRITE:
			return desktop::AccessMode::READ_WRITE;
		default:
			throw std::invalid_argument("The given option is not supported");
		}
	}
}