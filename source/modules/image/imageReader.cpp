#include "imageReader.h"
#include "formats/png.h"
#include "formats/bmp.h"

namespace img
{
	void ImageReader::readBMP(std::string filename, Image & image, FORCE_CHANNELS channel)
	{
		bmp::Bitmap bitmap;
		bitmap.load(platform, filename, channel);
		bitmap.toImage(image);
	}
	void ImageReader::readPNG(std::string filename, Image & image, FORCE_CHANNELS channel)
	{
		png::PNG png;
		png.load(platform, filename, channel);
		png.toImage(image);
		png.free();
	}
	
	Image ImageReader::readFile(std::string path, FORCE_CHANNELS channel)
	{
		std::string type = util::strip(path, ".").back();
		Image image;

		if (type == "bmp") {
			readBMP(path, image, channel);
		}
		else if (type == "png") {
			readPNG(path, image, channel);
		}
		else {
			throw sys::FileException(sys::FileError::BAD_FILE, "file type not supported", path);
		}

		return image;
	}

	Image ImageReader::readTexture(gl::Texture& texture, FORCE_CHANNELS channel)
	{
		std::vector<unsigned char> data;
		texture.getTextureImage(data);
		Image image(texture.getWidth(), texture.getHeight(), texture.getChannels(), data);
		return image;
	}
}