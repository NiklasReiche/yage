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
		std::string type = clib::strip(path, ".").back();
		if (type == "bmp") {
			writeBMP(path, image);
		}
		else {
			throw FileException(FileError::PATH_VIOLATION, path);
		}
	}
}