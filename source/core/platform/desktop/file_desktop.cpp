#include "file_desktop.h"

namespace desktop
{
	File::File(std::string & filename, AccessMode mode)
		: mode(mode), path(filename)
	{
		switch (mode)
		{
		case AccessMode::READ:
			fstream = std::make_unique<std::fstream>(filename, std::ios::in | std::ios::binary);
			break;
		case AccessMode::WRITE:
			fstream = std::make_unique<std::fstream>(filename, std::ios::out | std::ios::binary);
			break;
		case AccessMode::READ_WRITE:
			fstream = std::make_unique<std::fstream>(filename, std::ios::in | std::ios::out | std::ios::binary);
			break;
		}
	}
	File::File(File & other)
	{
		this->fstream = std::move(other.fstream);
		this->file_ptr = other.file_ptr;
		other.file_ptr = nullptr;
		this->mode = other.mode;
		this->path = other.path;
	}
	File::File(File && other)
	{
		this->fstream = std::move(other.fstream);
		this->file_ptr = other.file_ptr;
		other.file_ptr = nullptr;
		this->mode = other.mode;
		this->path = other.path;
	}
	File::~File()
	{
		fstream->close();
	}
	File& File::operator=(File & other)
	{
		this->fstream = std::move(other.fstream);
		this->file_ptr = other.file_ptr;
		other.file_ptr = nullptr;
		this->mode = other.mode;
		this->path = other.path;
	}

	void File::seek(int index, SeekOffset offset)
	{
		fstream->seekg(index, std::ios_base::seekdir(offset));
	}

	void File::read(void* buffer, size_t size)
	{
		if (mode == AccessMode::READ || mode == AccessMode::READ_WRITE) {
			fstream->read((char*)buffer, size);
		}
		else {
			throw sys::FileException(sys::FileError::ACCESS_VIOLATION, "", path);
		}
	}
	void File::read(std::stringstream & output)
	{
		std::streampos fileLength = 0;
		fstream->seekg(0, std::ios::beg);
		fileLength = fstream->tellg();
		fstream->seekg(0, std::ios::end);
		fileLength = fstream->tellg() - fileLength;

		// Allocate memory to read your file
		char* fileContent = new char[(size_t)fileLength + 1];
		// Read your file
		fstream->seekg(0, std::ios::beg);
		fstream->read(fileContent, fileLength);
		// For safety you can add a 0 terminating character at the end of your file ...
		fileContent[fileLength] = '\0';

		// Do whatever you want with the content of the file
		output = std::stringstream(fileContent);

		// Free the memoery you allocated earlier
		delete[] fileContent;
	}
	void File::read(std::vector<char> & output)
	{
		std::streampos fileLength = 0;
		fstream->seekg(0, std::ios::beg);
		fileLength = fstream->tellg();
		fstream->seekg(0, std::ios::end);
		fileLength = fstream->tellg() - fileLength;
	}

	void File::write(void* buffer, size_t size)
	{
		if (mode == AccessMode::WRITE || mode == AccessMode::READ_WRITE) {
			fstream->write((char*)buffer, size);
		}
		else {
			throw sys::FileException(sys::FileError::ACCESS_VIOLATION, "", path);
		}
	}

	void File::close()
	{
		fstream->close();
	}

	bool File::is_open()
	{
		return fstream->is_open();
	}

	bool File::get_error()
	{
		return (fstream->eof() || fstream->bad() || fstream->fail());
	}
}