#include <istream>

#include "IFile.h"

namespace platform
{
	class ITextFile : public IFile
	{
	public:
		~ITextFile() override = default;
	};
}