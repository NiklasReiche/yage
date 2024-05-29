#include "MovementListener.h"

#include "app.h"

void MovementListener::toggle_simulation()
{
    app->simulate = !app->simulate;
}

