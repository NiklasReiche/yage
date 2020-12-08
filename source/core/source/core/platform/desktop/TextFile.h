#pragma once

#include <core/platform/ITextFile.h>

#include "File.h"

namespace platform::desktop
{
	class TextFile : public File, public ITextFile
	{
	public:
		explicit TextFile(const std::string& filename, AccessMode mode = AccessMode::READ);

		std::string read() override;

		std::stringstream& readAll(std::stringstream& output) override;

		std::string readAll() override;
	};
}
