#pragma once

#include <android/asset_manager.h>
#include <cstdio>

namespace android
{
	enum class SeekOffset
	{
		BEG = SEEK_SET,
		CUR = SEEK_CUR
	};

	class Android_File
	{
	private:
		AAsset* asset;

	public:
		Android_File(AAsset* asset);
		~Android_File();

		void seek(int index, SeekOffset offset);
		void read(void* buffer, size_t size);
		void close();

		bool is_open();
	};
}