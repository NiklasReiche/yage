#include <runtime/Engine.h>

#include "BoxApp.h"

int main() {
	yage::Engine engine = yage::Engine(1500, 900, "Boxes");
	engine.register_application<BoxApp>();
	engine.run();
	return 0;
}
