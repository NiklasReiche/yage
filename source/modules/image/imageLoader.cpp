#include "imageLoader.h"

namespace img
{
	void ImageReader::readBMP(std::string filename, Image & image, FORCE_CHANNELS channel)
	{
		bmp::Bitmap bitmap;
		bitmap.load(platform, filename, channel);
		bitmap.toImage(image);
	}
	void ImageReader::readPNG(std::string filename, Image & image)
	{
		png::PNG png;
		png.load(platform, filename);
		png.toImage(image);
		png.free();
	}
	
	Image ImageReader::readFile(std::string path)
	{
		std::string type = clib::strip(path, ".").back();
		Image image;

		if (type == "bmp") {
			readBMP(path, image);
		}
		else if (type == "png") {
			readPNG(path, image);
		}
		else {
			throw FileException(FileError::PATH_VIOLATION, path);
		}

		return image;
	}


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