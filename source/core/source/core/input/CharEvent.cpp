#include "CharEvent.h"

namespace yage::input{
    CharEvent::CharEvent(unsigned int codepoint) : Event(Type::CHAR_EVENT), m_codepoint(codepoint)
    {
    }

    unsigned int CharEvent::codepoint() const
    {
        return m_codepoint;
    }
}
