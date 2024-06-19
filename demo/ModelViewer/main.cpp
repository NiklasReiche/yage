#include "app.h"

int main(int argc, char* argv[], char* []) {
	if (argc < 2) {
		std::cerr << "Please provide a path to a gltf file (.glb/.gltf)" << "\n";
		std::cerr << "usage: model_viewer <filepath>" << std::endl;
		return 1;
	}

	App app(argv[1]);
	app.run();
}
