#include "png_wrapper.h"
#include <png.h>
#include <core/platform/FileException.h>

namespace
{
	struct png_handles
	{
		png_structp png_ptr = nullptr;
		png_infop info_ptr = nullptr;
		png_infop end_info = nullptr;

		void free() {
			png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)&end_info);
		}
	};

	void user_error_fn(png_structp png_ptr, png_const_charp error_msg)
	{
		auto* png = (png_handles*)png_get_error_ptr(png_ptr);
		png->free();
		throw platform::FileException(platform::FileException::BAD_IO, error_msg, "");
	}

	void user_warning_fn(png_structp, png_const_charp warning_msg)
	{
		std::cout << warning_msg << std::endl;
	}

	void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		platform::IBinaryFile& file = *(platform::IBinaryFile*)png_get_io_ptr(png_ptr);
		file.read((void*)data, (size_t)length);
	}

	void checkSignature(platform::IBinaryFile& file)
	{
		const int PNG_SIG_SIZE = 8;

		std::vector<png_byte> signature(PNG_SIG_SIZE);
		file.read(&signature[0], PNG_SIG_SIZE);

		if (png_sig_cmp(&signature[0], 0, PNG_SIG_SIZE)) {
			throw platform::FileReadException("file is not a png image", "");
		}

		file.seek(0, platform::IFile::SeekOffset::BEG);
	}
}

namespace img::png
{
	Image read(platform::IBinaryFile& file, FORCE_CHANNELS forceChannel)
	{
		png_handles png;

		checkSignature(file);

		png.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)&png, user_error_fn, user_warning_fn);
		if (!png.png_ptr) {
			png.free();
			throw platform::FileException(platform::FileException::BAD_FILE, "file corrupted", "");
		}

		png.info_ptr = png_create_info_struct(png.png_ptr);
		if (!png.info_ptr) {
			png.free();
			throw platform::FileException(platform::FileException::BAD_FILE, "file corrupted", "");
		}

		png.end_info = png_create_info_struct(png.png_ptr);
		if (!png.end_info) {
			png.free();
			throw platform::FileException(platform::FileException::BAD_FILE, "file corrupted", "");
		}

		png_set_read_fn(png.png_ptr, (png_voidp)&file, user_read_data);

		png_read_info(png.png_ptr, png.info_ptr);
		unsigned int width = png_get_image_width(png.png_ptr, png.info_ptr);
		unsigned int height = png_get_image_height(png.png_ptr, png.info_ptr);
		int channels = png_get_channels(png.png_ptr, png.info_ptr);
		int bitDepth = png_get_bit_depth(png.png_ptr, png.info_ptr);
		unsigned int color = png_get_color_type(png.png_ptr, png.info_ptr);

		if ((color == PNG_COLOR_TYPE_RGB || color == PNG_COLOR_TYPE_GRAY) &&
		    (forceChannel == FORCE_CHANNELS::RGBA || forceChannel == FORCE_CHANNELS::GA)) {
			png_set_add_alpha(png.png_ptr, 0xff, PNG_FILLER_AFTER);
		}
		if ((color == PNG_COLOR_TYPE_GRAY || color == PNG_COLOR_TYPE_GRAY_ALPHA) &&
		    (forceChannel == FORCE_CHANNELS::RGB || forceChannel == FORCE_CHANNELS::RGBA)) {
			png_set_gray_to_rgb(png.png_ptr);
		}
		if ((color == PNG_COLOR_TYPE_RGB || color == PNG_COLOR_TYPE_RGB_ALPHA) &&
		    (forceChannel == FORCE_CHANNELS::G || forceChannel == FORCE_CHANNELS::GA)) {
			png_set_rgb_to_gray_fixed(png.png_ptr, 1, -1, -1);
		}
		if (color == PNG_COLOR_MASK_ALPHA &&
		    (forceChannel == FORCE_CHANNELS::RGB || forceChannel == FORCE_CHANNELS::G)) {
			png_set_strip_alpha(png.png_ptr);
		}

		png_read_update_info(png.png_ptr, png.info_ptr);
		bitDepth = png_get_bit_depth(png.png_ptr, png.info_ptr);

		//const unsigned int DATA_SIZE = height * width * channels * bitDepth / 8;
		const unsigned int ROW_STRIDE = width * channels * bitDepth / 8;

		Image image((int)width, (int)height, channels);

		/* pointers to the beginnings of each row in this->data */
		std::vector<png_bytep> rowPtrs(height);
		for (size_t i = 0; i < height; ++i) {
			png_uint_32 index = i * ROW_STRIDE;
			rowPtrs[i] = (png_bytep)image.data() + index;
		}

		png_read_image(png.png_ptr, &rowPtrs[0]);

		return image;
	}
}
