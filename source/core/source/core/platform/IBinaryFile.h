#pragma once

#include <cstddef>
#include <vector>

#include "IFile.h"

namespace yage::platform
{
	class IBinaryFile : public virtual IFile
	{
	public:
		~IBinaryFile() override = default;

		virtual void read(void* buffer, std::size_t size) = 0;

		virtual void write(void* buffer, std::size_t size) = 0;

        virtual std::vector<std::byte> read_all() = 0;
	};
}
