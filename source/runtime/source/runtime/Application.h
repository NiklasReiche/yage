#pragma once

namespace yage
{
    class Application
    {
    public:
        Application() = default;

        virtual ~Application() = default;

        virtual void initialize()
        {
        }

        virtual void pre_render_update()
        {
        }

        virtual void pre_physics_update()
        {
        }
    };
}
