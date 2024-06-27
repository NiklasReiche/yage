#include "app.h"
#include "utils/NotImplementedException.h"

int main(const int argc, char* argv[], char* []) {
    throw NotImplementedException();

	if (argc < 2) {
		std::cerr << "Please provide a path to a gltf file (.glb/.gltf)" << "\n";
		std::cerr << "usage: model_viewer <filepath>" << std::endl;
		return 1;
	}

	App app(argv[1]);
	app.run();
}
