#include "Desktop_File.h"

namespace glfw
{
	Desktop_File::Desktop_File(std::string & filename) 
	{
		fstream = std::make_unique<std::ifstream>(std::ifstream(filename, std::ios::binary));
	}
	Desktop_File::Desktop_File(Desktop_File & desktopFile)
	{
		fstream = std::move(desktopFile.fstream);
	}
	Desktop_File::~Desktop_File()
	{
		fstream->close();
	}

	void Desktop_File::seek(int index, SeekOffset offset)
	{
		fstream->seekg(index, (int)offset);
	}

	void Desktop_File::read(void* buffer, size_t size)
	{
		fstream->read((char*)buffer, size);
	}

	void Desktop_File::close()
	{
		fstream->close();
	}

	bool Desktop_File::is_open()
	{
		return fstream->is_open();
	}
}