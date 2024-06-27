#include <runtime/Engine.h>

#include "BilliardsApp.h"

int main() {
    yage::Engine engine = yage::Engine(1500, 900, "Billiards");
    engine.register_application<BilliardsApp>();
    engine.run();
    return 0;
}
