#pragma once

#include <unordered_map>
#include <memory>

#include "Loader.h"

namespace yage::res
{
    template<typename ResourceType>
    class Resource;

    template<typename ResourceType>
    class Store final
    {
    public:
        explicit Store(std::unique_ptr<Loader<ResourceType>> loader) : m_loader(std::move(loader))
        {
        }

        Resource<ResourceType> load_resource(const std::string& uri);

    private:
        std::unique_ptr<Loader<ResourceType>> m_loader;
        std::unordered_map<std::string, ResourceType> resources;

        ResourceType& get(const std::string& uri)
        {
            if (resources.contains(uri)) {
                return resources[uri];
            }

            load_resource(uri);
            return resources[uri];
        }

        friend class Resource<ResourceType>;
    };
}

#include "Resource.h"

namespace yage::res
{
    template<typename ResourceType>
    Resource<ResourceType> Store<ResourceType>::load_resource(const std::string& uri)
    {
        resources.insert(std::make_pair(uri, m_loader->load(uri)));
        return Resource(uri, this);
    }
}
