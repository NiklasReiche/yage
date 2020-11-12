#include <core/platform/ITextFile.h>
#include "File.h"

namespace platform::desktop
{
	class TextFile : public virtual File, public ITextFile
	{
	public:
		explicit TextFile(const std::string& filename, AccessMode mode = AccessMode::READ);
	};
}
