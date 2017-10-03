#include "imageWriter.h"
#include "formats/png.h"
#include "formats/bmp.h"

namespace img
{
	void ImageWriter::writeBMP(std::string filename, Image & image, FORCE_CHANNELS channel)
	{
		bmp::Bitmap bitmap;
		bitmap.fromImage(image);
		bitmap.save(platform, filename, channel);
	}

	void ImageWriter::writeFile(Image& image, std::string path)
	{
		std::string type = util::strip(path, ".").back();
		if (type == "bmp") {
			writeBMP(path, image);
		}
		else {
			throw sys::FileException(sys::FileError::BAD_FILE, "file type not supported", path);
		}
	}
}