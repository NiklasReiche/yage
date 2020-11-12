#pragma once

#include <core/platform/IBinaryFile.h>
#include <image/Image.h>
#include <image/enum.h>

namespace img::png
{
	Image read(platform::IBinaryFile& file, FORCE_CHANNELS forceChannel = FORCE_CHANNELS::AUTO);
}
