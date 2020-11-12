#pragma once

#include <cstddef>

#include "IFile.h"

namespace platform
{
	class IBinaryFile : public virtual IFile
	{
	public:
		~IBinaryFile() override = default;

		virtual void read(void* buffer, size_t size) = 0;

		virtual void write(void* buffer, size_t size) = 0;
	};
}
