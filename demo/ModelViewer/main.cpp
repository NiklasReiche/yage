#include "app.h"

int main(int argc, char* argv[], char* []) {
	if (argc < 2) {
		return 1;
	}

	App app(argv[1]);
	app.run();
}
