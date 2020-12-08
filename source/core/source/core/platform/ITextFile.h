#pragma once

#include <sstream>

#include "IFile.h"

namespace platform
{
	class ITextFile : public virtual IFile
	{
	public:
		~ITextFile() override = default;

		virtual std::string read() = 0;

		virtual std::stringstream& readAll(std::stringstream& output) = 0;

		virtual std::string readAll() = 0;
	};
}