#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>

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

        std::vector<Resource<ResourceType>> load_archive(std::span<std::string> uris);

        std::vector<Resource<ResourceType>> load_archive(const std::string& uri);

    private:
        std::unique_ptr<Loader<ResourceType>> m_loader;
        std::unordered_map<std::string, ResourceType> m_resources;

        ResourceType& get(const std::string& uri)
        {
            if (m_resources.contains(uri)) {
                return m_resources[uri];
            }

            load_resource(uri);
            return m_resources[uri];
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
        m_resources.insert(std::make_pair(uri, m_loader->load_resource(uri)));
        return Resource(uri, this);
    }

    template<typename ResourceType>
    std::vector<Resource<ResourceType>> Store<ResourceType>::load_archive(std::span<std::string> uris)
    {
        std::vector<ResourceType> resources = m_loader->load_archive(uris);
        assert(resources.size() == uris.size());

        std::vector<Resource<ResourceType>> handles;
        handles.reserve(resources.size());
        for (int i = 0; i < resources.size(); ++i) {
            m_resources.insert(std::make_pair(uris[i], resources[i]));
            handles.emplace_back(uris[i], this);
        }
        return handles;
    }

    template<typename ResourceType>
    std::vector<Resource<ResourceType>> Store<ResourceType>::load_archive(const std::string& uri)
    {
        const auto& [resources, uris] = m_loader->load_archive(uri);
        assert(resources.size() == uris.size());

        std::vector<Resource<ResourceType>> handles;
        handles.reserve(resources.size());
        for (int i = 0; i < resources.size(); ++i) {
            m_resources.insert(std::make_pair(uris[i], resources[i]));
            handles.emplace_back(uris[i], this);
        }
        return handles;
    }
}
