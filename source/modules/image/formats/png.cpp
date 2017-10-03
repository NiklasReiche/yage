#include "png.h"

namespace img
{
namespace png
{
	void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		png_voidp io_ptr = png_get_io_ptr(png_ptr);
		if (io_ptr == NULL)
			return;   // add custom error handling here

		sys::File& file = *(sys::File*)io_ptr;

		file.read((void*)data, (size_t)length);
	}
	void user_error_fn(png_structp png_ptr, png_const_charp error_msg)
	{
		PNG* png = (PNG*)png_get_error_ptr(png_ptr);
		png->free();
		std::cout << error_msg << std::endl;
		throw FileException(FileError::UNKNOWN);
	}
	void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
	{
		std::cout << warning_msg << std::endl;
	}

	bool PNG::isValid(sys::File & file)
	{
		std::vector<png_byte> signature(PNG_SIG_SIZE);
		file.read(&signature[0], PNG_SIG_SIZE);

		int is_png = png_sig_cmp(&signature[0], 0, PNG_SIG_SIZE);
		/* for some reason this does not work. so we're manually seeking*/
		//png_set_sig_bytes(png_ptr, PNG_SIG_SIZE);
		file.seek(0, sys::SeekOffset::BEG);

		return (is_png == 0);
	}

	void PNG::free()
	{
		if (rowPtrs) {
			delete[](png_bytep)rowPtrs;
		}
		png_destroy_read_struct((png_structpp)&png_ptr, (png_infopp)&info_ptr, (png_infopp)&end_info);
	}

	void PNG::readInfoHeader()
	{
		this->width = png_get_image_width(png_ptr, info_ptr);
		this->height = png_get_image_height(png_ptr, info_ptr);
		this->bitdepth = png_get_bit_depth(png_ptr, info_ptr);
		this->channels = png_get_channels(png_ptr, info_ptr);
		this->color = png_get_color_type(png_ptr, info_ptr);
	}

	void PNG::readImageData()
	{
		const unsigned int DATA_SIZE = height * width * channels * bitdepth / 8;
		const unsigned int ROW_STRIDE = width * channels * bitdepth / 8;

		this->data.resize(DATA_SIZE);

		/* pointers to the beginnings of each row in this->data */
		this->rowPtrs = new png_bytep[height];

		for (size_t i = 0; i < height; ++i) {
			png_uint_32 index = i * ROW_STRIDE;
			this->rowPtrs[i] = (png_bytep)&this->data[0] + index;
		}

		png_read_image(png_ptr, rowPtrs);
	}

	void PNG::load(sys::PlatformHandle* platform, std::string filename, FORCE_CHANNELS forceChannel)
	{
		sys::File file = platform->open(filename);

		if (!isValid(file)) {
			throw FileException(FileError::UNKNOWN, filename);
		}

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)this, user_error_fn, user_warning_fn);
		if (!png_ptr) {
			this->free();
			throw FileException(FileError::UNKNOWN, filename);
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			this->free();
			throw FileException(FileError::UNKNOWN, filename);
		}

		end_info = png_create_info_struct(png_ptr);
		if (!end_info) {
			this->free();
			throw FileException(FileError::UNKNOWN, filename);
		}

		png_set_read_fn(png_ptr, (png_voidp)&file, user_read_data);

		png_read_info(png_ptr, info_ptr);
		readInfoHeader();

		if ((color == PNG_COLOR_TYPE_RGB || color == PNG_COLOR_TYPE_GRAY) && (forceChannel == FORCE_CHANNELS::RGBA || forceChannel == FORCE_CHANNELS::GA)) {
			png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
		}
		if ((color == PNG_COLOR_TYPE_GRAY || color == PNG_COLOR_TYPE_GRAY_ALPHA) && (forceChannel == FORCE_CHANNELS::RGB || forceChannel == FORCE_CHANNELS::RGBA)) {
			png_set_gray_to_rgb(png_ptr);
		}
		if ((color == PNG_COLOR_TYPE_RGB || color == PNG_COLOR_TYPE_RGB_ALPHA) && (forceChannel == FORCE_CHANNELS::G || forceChannel == FORCE_CHANNELS::GA)) {
			png_set_rgb_to_gray_fixed(png_ptr, 1, -1, -1);
		}

		png_read_update_info(png_ptr, info_ptr);
		readInfoHeader();

		readImageData();
		
		file.close();
	}

	void PNG::toImage(Image & image)
	{
		image = Image(width, height, channels, data);
	}
}
}