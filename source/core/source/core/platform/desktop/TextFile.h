#pragma once

#include <core/platform/ITextFile.h>

#include "File.h"

namespace yage::platform::desktop
{
	class TextFile final : public File, public ITextFile
	{
	public:
		explicit TextFile(const std::string& filename, AccessMode mode = AccessMode::READ);

		std::string read() override;

		std::string read_line() override;

		std::stringstream& read_all(std::stringstream& output) override;

		std::string read_all() override;
	};
}
