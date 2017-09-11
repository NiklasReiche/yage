#include <image/imageLoader.h>
#include <platform/Platform.h>
#include <zlib/zlib.h>
#include <libpng/png.h>


void readFile(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_voidp io_ptr = png_get_io_ptr(png_ptr);
	if (io_ptr == NULL)
		return;   // add custom error handling here

	sys::File& file = *(sys::File*)io_ptr;

	file.read((void*)data, (size_t)length);
}



int main()
{
	sys::PlatformHandle platform;

	img::ImageReader reader(&platform);
	img::ImageWriter writer(&platform);

	std::cout << "Read Start" << std::endl;
	img::Image file_1 = reader.readFile("../../../Tests/image/example_3.png");
	std::cout << "Read End" << std::endl;

	std::cout << "Write Start" << std::endl;
	writer.writeFile(file_1, "../../../Tests/image/example_2.bmp");
	std::cout << "Write End" << std::endl;


	std::cout << "End" << std::endl;
	std::cin.get();
}