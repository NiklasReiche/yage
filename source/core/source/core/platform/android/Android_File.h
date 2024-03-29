#pragma once

#include <android/asset_manager.h>
#include <cstdio>
#include <sstream>
#include <exception>

#include "../util.h"

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
		AAsset* asset = nullptr;
		FILE* file = nullptr;

	public:
		Android_File(AAsset* asset);
		Android_File(FILE* file);
		~Android_File();

		void seek(int index, SeekOffset offset);
		void read(void* buffer, size_t size);
		void read(std::stringstream & output);
		void close();

		bool is_open();
	};
}