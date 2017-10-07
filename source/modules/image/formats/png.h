#pragma once

#include <vector>
#include <string>

#include <libpng/png.h>

#include <platform/platform.h>

#include "../image.h"
#include "../enum.h"

namespace img 
{
namespace png 
{
	void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length);
	void user_error_fn(png_structp png_ptr, png_const_charp error_msg);
	void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg);


	class PNG
	{
	private:
		/* size of the png signature*/
		const int PNG_SIG_SIZE = 8;
		/* handles for libpng */
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		png_infop end_info = NULL;
		png_bytep* rowPtrs = NULL;

		/* width & hight of the image */
		unsigned int width = 0;
		unsigned int height = 0;
		/* number of channels */
		int channels = 0;
		/* number of bits per channel */
		int bitdepth = 0;
		/* color type */
		unsigned int color = 0;
		/* image data */
		std::vector<unsigned char> data;

		bool isValid(sys::File & file);
		void readInfoHeader();
		void readImageData();

	public:
		void load(sys::PlatformHandle* platform, std::string filename, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);
		void toImage(Image & image);
		void free();

	};
}
}