#pragma once

#include <core/platform/IBinaryFile.h>

#include "Image.h"
#include "enum.h"

namespace yage::img
{
	Image readFromFile(platform::IBinaryFile& file, FORCE_CHANNELS forceChannels = FORCE_CHANNELS::AUTO);

	void writeToFile(platform::IBinaryFile& file, const Image& image, FORCE_CHANNELS forceChannels = FORCE_CHANNELS::AUTO);
}
