#pragma once

#include <cstdint>
#include <memory>
#include <utility>

namespace res
{
    template<typename ResourceType>
    class Store;

    template<typename ResourceType>
    class Resource final
    {
    public:
        ResourceType& get();

    private:
        std::size_t key;
        Store<ResourceType>* store; // TODO: pointer type (if we use raw pointer, we should add custom validation)

        Resource(std::size_t key, Store<ResourceType>* store);

        friend class Store<ResourceType>; // TODO: is there a better way to do this?
    };
}

#include "Store.h"

namespace res
{
    template<typename ResourceType>
    Resource<ResourceType>::Resource(std::size_t key, Store<ResourceType>* store) : key(key), store(store)
    {}

    template<typename ResourceType>
    ResourceType& Resource<ResourceType>::get()
    {
        return store->getUnderlyingResource(key);
    }
}
