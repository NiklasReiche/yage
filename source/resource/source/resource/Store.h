#pragma once

#include <cstdint>
#include <unordered_map>

#include "Loader.h"

namespace res
{
    template<typename ResourceType>
    class Resource;

    // TODO: should the loader be coupled to the store? Would make reloading easier (caching/hotswap). Would mean a 1-to-1 relationship between stores and loaders.
    // TODO: alternatively, store the loader with the resource handle
    template<typename ResourceType>
    class Store final
    {
    public:
        template<typename LocationType>
        Resource<ResourceType> loadResource(Loader<ResourceType, LocationType>& loader, const LocationType& location);

    private:
        std::unordered_map<std::size_t, ResourceType> resources;

        ResourceType& getUnderlyingResource(std::size_t key);

        friend class Resource<ResourceType>;
    };
}

#include "Resource.h"

namespace res
{
    template<typename ResourceType>
    ResourceType& Store<ResourceType>::getUnderlyingResource(std::size_t key)
    {
        return resources.at(key);
    }

    template<typename ResourceType>
    template<typename LocationType>
    Resource<ResourceType>
    Store<ResourceType>::loadResource(Loader<ResourceType, LocationType>& loader, const LocationType& location)
    {
        std::size_t key = std::hash<LocationType>{}(location);
        resources.insert({key, loader.load(location)});
        return Resource(key, this);
    }
}
