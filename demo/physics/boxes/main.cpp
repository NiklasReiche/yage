#include <runtime/Engine.h>

#include "NewApp.h"

int main() {
	yage::Engine engine = yage::Engine(1500, 900, "Boxes");
	engine.register_application<NewApp>();
	engine.run();
	return 0;
}
