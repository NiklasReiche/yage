#include <image\img.h>

int main()
{
	img::Image bitmap1;
	img::readBMP("../../../Tests/image/example_1.bmp", bitmap1, img::FORCE_CHANNELS::AUTO);

	std::cout << bitmap1 << std::endl;

	img::writeBMP("../../../Tests/image/example_2.bmp", bitmap1);
	img::readBMP("../../../Tests/image/example_2.bmp", bitmap1, img::FORCE_CHANNELS::AUTO);

	std::cout << bitmap1 << std::endl;

	std::cin.get();
}