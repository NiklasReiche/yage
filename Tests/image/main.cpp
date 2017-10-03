#include <image/imageLoader.h>
#include <platform/Platform.h>


int main()
{
	sys::PlatformHandle platform;

	img::ImageReader reader(&platform);
	img::ImageWriter writer(&platform);

	img::Image file_1 = reader.readFile("../../../Tests/image/example_1.png");
	std::cout << file_1 << std::endl;

	file_1.addAlpha();

	std::cout << file_1 << std::endl;

	std::cout << "Write Start" << std::endl;
	writer.writeFile(file_1, "../../../Tests/image/example_2.bmp");
	std::cout << "Write End" << std::endl;


	std::cout << "End" << std::endl;
	std::cin.get();
}