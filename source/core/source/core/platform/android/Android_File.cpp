#include "Android_File.h"

namespace android
{
	Android_File::Android_File(AAsset* asset)
		: asset(asset) {}
	Android_File::Android_File(FILE* file)
		: file(file) {}
	Android_File::~Android_File()
	{
		if (asset != nullptr) {
			AAsset_close(asset);
		}
		else if (file != nullptr) {
			std::fclose(file);
		}
	}

	void Android_File::seek(int index, SeekOffset offset)
	{
		if (asset != nullptr) {
			AAsset_seek(asset, index, (int)offset);
		}
		else if (file != nullptr) {
			std::fseek(file, index, (int)offset);
		}
		else {
			throw std::runtime_error("File error: handle is null");
		}
	}

	void Android_File::read(void* buffer, size_t size)
	{
		if (asset != nullptr) {
			AAsset_read(asset, buffer, size);
		}
		else if (file != nullptr) {
			if (std::fread(buffer, size, 1, file) != 1) {
				throw std::runtime_error("File error: could not read " + util::toString(size) + " bytes");
			};
		}
		else {
			throw std::runtime_error("File error: handle is null");
		}
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

		// Free the memory allocated earlier
		delete[] fileContent;
	}

	void Android_File::close()
	{
		if (asset != nullptr) {
			AAsset_close(asset);
		}
		else if (file != nullptr) {
			std::fclose(file);
		}
	}

	bool Android_File::is_open()
	{
		if (asset != nullptr || file != nullptr) {
			return true;
		}
		return false;
	}
}