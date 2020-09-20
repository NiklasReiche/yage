#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "../FileException.h"
#include "../File.h"

namespace sys::desktop
{
	enum class AccessMode
	{
		READ = std::ios::in,
		WRITE = std::ios::out,
		READ_WRITE,
	};
	enum class SeekOffset
	{
		BEG = std::ios::beg,
		CUR = std::ios::cur
	};

	class File final : public IFile
	{
	public:
		explicit File(const std::string & filename, AccessMode mode = AccessMode::READ);
		~File() = default;
		File(const File& other) = delete;
		File(File&& other) noexcept;
		File& operator=(const File& other) = delete;
		File& operator=(File&& other) noexcept;

		void seek(int index, SeekOffset offset) override;

		void read(void* buffer, size_t size) override;
		void read(std::stringstream & output) override;

		void write(void* buffer, size_t size) override;
		void write(std::stringstream & data) override;

		void close() override;

		bool isOpen() override;
		bool getError() const;

	private:
		std::string path;
		std::unique_ptr<std::fstream> fileStream;
		AccessMode mode;

		static desktop::SeekOffset convertSeekOffset(SeekOffset offset);
		static desktop::AccessMode convertAccessMode(AccessMode mode);
	};
}
