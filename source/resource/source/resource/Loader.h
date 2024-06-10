#pragma once

namespace yage::res
{
    template<typename ResourceType, typename LocationType>
    class Loader
    {
    public:
        virtual ~Loader() = default;

        virtual ResourceType load(LocationType location) = 0;
    };
}
