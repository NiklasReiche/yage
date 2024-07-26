#include "img.h"
#include "formats/bmp.h"
#include "formats/png_wrapper.h"
#include <utils/strings.h>

namespace yage::img
{
	Image readFromFile(platform::IBinaryFile& file, FORCE_CHANNELS forceChannels)
	{
		std::string fileExtension = utils::strip(file.file_name(), ".").back();
		if (fileExtension == "png") {
			return img::png::read(file, forceChannels);
		} else if (fileExtension == "bmp") {
			bmp::Bitmap bitmap;
			bitmap.load(file, forceChannels);
			return bitmap.toImage();
		} else {
			throw std::runtime_error("image files of type " + fileExtension + " are not supported");
		}
	}

	void writeToFile(platform::IBinaryFile& file, const Image& image, FORCE_CHANNELS forceChannels)
	{
		std::string fileExtension = utils::strip(file.file_name(), ".").back();
		if (fileExtension == "png") {
			throw std::runtime_error("image files of type " + fileExtension + " are not supported");
		} else if (fileExtension == "bmp") {
			bmp::Bitmap bitmap;
			bitmap.fromImage(image);
			bitmap.save(file, forceChannels);
		} else {
			throw std::runtime_error("image files of type " + fileExtension + " are not supported");
		}
	}
}