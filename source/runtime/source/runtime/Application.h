#pragma once

namespace yage
{
    class Engine;

    class Application
    {
    public:
        explicit Application() = default;

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

    protected:
        Engine* m_engine = nullptr;

    private:
        friend class Engine;
    };
}
