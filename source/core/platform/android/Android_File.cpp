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

	void Android_File::close()
	{
		AAsset_close(asset);
	}

	bool Android_File::is_open()
	{
		return true;
	}
}