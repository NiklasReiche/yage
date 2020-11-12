#include <string>
#include <ostream>

#include "IFile.h"

namespace platform
{
	class ITextFile : public IFile
	{
	public:
		~ITextFile() override = default;

		friend ITextFile& operator<<(ITextFile& os, const std::string& data);
	};

	ITextFile& operator<<(ITextFile& os, const std::string& data);
}