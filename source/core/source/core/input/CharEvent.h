#pragma once

#include "Event.h"

namespace input
{
    class CharEvent : public Event
    {
    public:
        explicit CharEvent(unsigned int codepoint);

        [[nodiscard]]
        unsigned int codepoint() const;

    private:
        unsigned int m_codepoint;
    };
}
