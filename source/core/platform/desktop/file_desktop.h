#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "../exception.h"

namespace desktop
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

	class File
	{
	private:
		std::string path;
		FILE* file_ptr;
		std::unique_ptr<std::fstream> fstream;
		AccessMode mode;

	public:
		File(std::string & filename, AccessMode mode = AccessMode::READ);
		File(File & other);
		File(File && other);
		~File();
		File& operator=(File & other);

		void seek(int index, SeekOffset offset);
		void read(void* buffer, size_t size);
		void read(std::stringstream & output);
		void read(std::vector<char> & output);
		void write(void* buffer, size_t size);
		void close();

		bool is_open();
		bool get_error();
	};
}