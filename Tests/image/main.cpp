#include <image\img.h>

int main()
{
	img::Image bitmap1;
	img::readBMP("../../../Tests/image/example_1.bmp", bitmap1);
	std::cout << bitmap1 << std::endl;

	std::cin.get();
}