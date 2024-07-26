#pragma once

#include <core/platform/IBinaryFile.h>

#include "File.h"

namespace yage::platform::desktop
{
	class BinaryFile final : public File, public IBinaryFile
	{
	public:
		explicit BinaryFile(const std::string& filename, AccessMode mode = AccessMode::READ);

		void read(void* buffer, std::size_t size) override;

		void write(void* buffer, std::size_t size) override;

        std::vector<std::byte> read_all() override;
	};
}
