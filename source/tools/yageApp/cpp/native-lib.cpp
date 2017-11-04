#include <sstream>
#include <vector>
#include <memory>

#include <platform/Platform.h>
#include <graphics/Graphics.h>
#include <input/InputController.h>
#include <math/MVector.h>

#include <graphics/GLES2/GLES2_Enum.h>

class Game
{
private:
    sys::PlatformHandle* platformHandle;
    gl::GraphicsContext* glContext;
    input::InputController* inputController;

    bool running = false;

public:
    Game(android_app* app)
    {
        platformHandle = new sys::PlatformHandle(app);
        platformHandle->callbacks.onAppStop = std::bind(&Game::stop, this);
        platformHandle->callbacks.onAppStart = std::bind(&Game::load, this);

        //load();
        //run();
    }
    ~Game()
    {
        delete platformHandle;
    }

    void load()
    {
        platformHandle->log("LOAD");

        glContext = new gl::GraphicsContext(platformHandle);
        glClearColor(0.5, 0.5, 0, 1);
        inputController = new input::InputController(platformHandle);

        running = true;
    }

    void run()
    {
        platformHandle->getTimeStep();

        while(!platformHandle->shouldDestroy())
        {
            if (running){
                double dt = platformHandle->getTimeStep();
                glClear(GL_COLOR_BUFFER_BIT);

                glContext->swapBuffers();
                inputController->poll();
            }
            else{
                platformHandle->pollEvents();
            }
        }
    }

    void stop()
    {
        platformHandle->log("STOP");

        delete glContext;
        delete inputController;

        running = false;
    }
};

void android_main(android_app* app)
{
    Game game(app);
    game.run();
}
