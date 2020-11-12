#include <utils/utils.h>
#include "imageWriter.h"
#include "formats/png_format.h"
#include "formats/bmp.h"

namespace img
{
	void ImageReaderWriter::writeBmp(const std::string & filename, Image & image, const FORCE_CHANNELS channel)
	{
		bmp::Bitmap bitmap;
		bitmap.fromImage(image);
		bitmap.save(filename, channel);
	}

	void ImageReaderWriter::writeFile(Image & image, const std::string & path)
	{
		const std::string type = util::strip(path, ".").back();
		if (type == "bmp") {
			writeBmp(path, image);
		}
		else {
			throw sys::FileException(sys::FileException::BAD_FILE, "file type not supported", path);
		}
	}
}
