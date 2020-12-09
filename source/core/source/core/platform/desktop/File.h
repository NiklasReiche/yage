#pragma once

#include <fstream>
#include <memory>

#include <core/platform/FileException.h>
#include <core/platform/IFile.h>

namespace platform::desktop
{
	enum class FileType
	{
		BINARY = std::ios::binary,
		TEXT
	};

	class File : public virtual IFile
	{
	public:
		explicit File(const std::string & filename, FileType type, AccessMode mode);
		~File() override = default;

		void seek(int index, SeekOffset offset) override;

		std::string getFileName() override;

		bool eof() override;

	protected:
		std::string filename;
		std::unique_ptr<std::fstream> fileStream;
		AccessMode mode;

	private:
		static int convertSeekOffset(SeekOffset offset);
		static int convertAccessMode(AccessMode mode);
	};
}
