#pragma once

#include <sstream>

#include "IFile.h"

namespace yage::platform
{
	class ITextFile : public virtual IFile
	{
	public:
		~ITextFile() override = default;

		virtual std::string read() = 0;

		virtual std::string read_line() = 0;

		virtual std::stringstream& read_all(std::stringstream& output) = 0;

		virtual std::string read_all() = 0;
	};
}