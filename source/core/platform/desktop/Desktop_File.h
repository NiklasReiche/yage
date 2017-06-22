#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <memory>

namespace glfw
{
	enum class SeekOffset
	{
		BEG = std::ios::beg,
		CUR = std::ios::cur
	};

	class Desktop_File
	{
	private:
		std::unique_ptr<std::ifstream> fstream;

	public:
		Desktop_File(Desktop_File & desktopFile);
		Desktop_File(std::string & filename);
		~Desktop_File();

		void seek(int index, SeekOffset offset);
		void read(void* buffer, size_t size);
		void read(std::stringstream & output);
		void close();

		bool is_open();
	};
}