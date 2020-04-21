#pragma once

#include <sstream>

namespace sys
{
	class IFile;
	typedef std::unique_ptr<IFile> File;

	class IFile
	{
	public:
		enum class AccessMode
		{
			READ, WRITE, READ_WRITE
		};
		enum class SeekOffset
		{
			BEG, CUR
		};

		virtual ~IFile() = default;

		virtual void seek(int index, SeekOffset offset) = 0;
		
		virtual void read(void* buffer, size_t size) = 0;
		virtual void read(std::stringstream & output) = 0;

		virtual void write(void* buffer, size_t size) = 0;
		virtual void write(std::stringstream & data) = 0;

		virtual void close() = 0;
		virtual bool isOpen() = 0;
	};
}
