#include <image/imageLoader.h>
#include <platform/Platform.h>

int main()
{
	sys::PlatformHandle platform;

	img::ImageReader reader(&platform);
	img::ImageWriter writer(&platform);

	std::cout << "Start" << std::endl;
	img::Image bitmap_1 = reader.readFile("../../../Tests/image/back.bmp");
	std::cout << "End" << std::endl;

	writer.writeFile(bitmap_1, "../../../Tests/image/example_2.bmp");


	//img::Image bitmap_2 = reader.readFile("../../../Tests/image/example_2.bmp");

	//std::cout << bitmap_2 << std::endl;
	std::cout << "End" << std::endl;

	std::cin.get();
}