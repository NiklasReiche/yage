#include <runtime/Engine.h>

#include "ModelViewerApp.h"

int main() {
	yage::Engine engine = yage::Engine(1500, 900, "Model Viewer");
    engine.register_application<ModelViewerApp>();
    engine.run();
    return 0;
}
