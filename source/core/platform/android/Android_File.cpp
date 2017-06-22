#include "Android_File.h"

namespace android
{
	Android_File::Android_File(AAsset* asset)
		: asset(asset) {}
	Android_File::~Android_File()
	{
		AAsset_close(asset);
	}

	void Android_File::seek(int index, SeekOffset offset)
	{
		AAsset_seek(asset, index, (int)offset);
	}

	void Android_File::read(void* buffer, size_t size)
	{
		AAsset_read(asset, buffer, size);
	}

	void Android_File::read(std::stringstream & output)
	{
		size_t fileLength = AAsset_getLength(asset);

		// Allocate memory to read your file
		char* fileContent = new char[fileLength + 1];
		// Read your file
		AAsset_seek(asset, 0, (int)SeekOffset::BEG);
		AAsset_read(asset, fileContent, fileLength);
		// For safety you can add a 0 terminating character at the end of your file ...
		fileContent[fileLength] = '\0';

		// Do whatever you want with the content of the file
		output = std::stringstream(fileContent);

		// Free the memoery you allocated earlier
		delete[] fileContent;
	}

	void Android_File::close()
	{
		AAsset_close(asset);
	}

	bool Android_File::is_open()
	{
		return true;
	}
}