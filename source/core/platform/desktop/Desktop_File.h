#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "../Exception.h"

namespace glfw
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

	class Desktop_File
	{
	private:
		std::string path;
		FILE* file_ptr;
		std::unique_ptr<std::fstream> fstream;
		AccessMode mode;

	public:
		Desktop_File(std::string & filename, AccessMode mode = AccessMode::READ);
		Desktop_File(Desktop_File & desktopFile);
		~Desktop_File();

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