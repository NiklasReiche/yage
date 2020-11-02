#include <utils/utils.h>
#include "imageReader.h"
#include "formats/png.h"
#include "formats/bmp.h"
#include <utils/NotImplementedException.h>

namespace img
{
	Image ImageReader::readFile(const std::string & path, const FORCE_CHANNELS channel)
	{
		const std::string type = util::strip(path, ".").back();
		Image image;

		if (type == "bmp") {
			readBmp(path, image, channel);
		}
		else if (type == "png") {
			readPng(path, image, channel);
		}
		else {
			throw sys::FileException(sys::FileException::BAD_FILE, "file type not supported", path);
		}

		return image;
	}

	Image ImageReader::readTexture(const gl::Texture2D & texture, const FORCE_CHANNELS channel)
	{
		throw NotImplementedException();
#if 0
		std::vector<unsigned char> data;
		texture->getTextureImage(data);
		Image image(texture->getWidth(), texture->getHeight(), texture->getChannels(), data);
		return image;
#endif
	}

	void ImageReader::readBmp(const std::string & filename, Image & image, FORCE_CHANNELS channel)
	{
		bmp::Bitmap bitmap;
		bitmap.load(filename, channel);
		bitmap.toImage(image);
	}
	void ImageReader::readPng(const std::string & filename, Image & image, FORCE_CHANNELS channel)
	{
		png::PNG png;
		png.load(filename, channel);
		png.toImage(image);
		png.free();
	}
}
