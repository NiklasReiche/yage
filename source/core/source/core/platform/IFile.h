#pragma once

#include <string>

namespace platform
{
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

		virtual std::string getFileName() = 0;

		virtual bool eof() = 0;
	};
}
