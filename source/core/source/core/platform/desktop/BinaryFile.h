#pragma once

#include <core/platform/IBinaryFile.h>

#include "File.h"

namespace platform::desktop
{
	class BinaryFile : public File, public IBinaryFile
	{
	public:
		explicit BinaryFile(const std::string& filename, AccessMode mode = AccessMode::READ);

		void read(void* buffer, size_t size) override;

		void write(void* buffer, size_t size) override;
	};
}
