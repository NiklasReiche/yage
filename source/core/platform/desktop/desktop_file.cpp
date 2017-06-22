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
	void Desktop_File::read(std::stringstream & output)
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

	void Desktop_File::close()
	{
		fstream->close();
	}

	bool Desktop_File::is_open()
	{
		return fstream->is_open();
	}
}