#pragma once

#include <string>

namespace yage::res
{
    template<typename ResourceType>
    class Store;

    template<typename ResourceType>
    class Loader
    {
    public:
        virtual ~Loader() = default;

    protected:
        virtual ResourceType load(const std::string& uri) = 0;

    private:
        friend class Store<ResourceType>;
    };
}
